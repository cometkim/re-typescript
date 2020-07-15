// TODO: Eventually define an interface so that multiple backends can be built
// ALso: Plugin system should be solved here as well, before the AST is built
// Could use a similar system to graphql-ppx (see https://github.com/reasonml-community/graphql-ppx/blob/master/src/base/validations.re for reference)
open Ast;
open Node;
module Exceptions = {
  exception UnexpectedAtThisPoint(string);
};

// ---------------------------------------------------------------- ´--------------------------
// ------------------------------------------------------------------------------------------
// --- Utility types
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
open Parser_utils;

// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Entry / Preparation
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
let rec parse__Entry = (~source_files: array(Ts_morph.SourceFile.t)) => {
  let runtime = {
    root_modules: [||],
    node_count: 0,
    parse_config: {
      exports_only: false,
    },
  };
  let runtime =
    source_files
    |> CCArray.fold_left(
         (runtime, source_file) => {
           let scope = {
             root_declarations: [||],
             source_file: None,
             parent: None,
             path: [||],
             has_any: false,
           };

           let source_file =
             source_file
             |> Ts_morph.SourceFile.castToNode
             |> Ts_nodes.Generic.fromMorphNode
             |> Ts_nodes.SourceFile.fromGeneric;
           let (runtime, _, t) =
             parse__Node__SourceFile(~runtime, ~scope, source_file);
           runtime |> Runtime.add_root_module(t);
         },
         runtime,
       );
  runtime.root_modules;
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Generic Node ("Hub")
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Generic =
    (~runtime, ~scope, node: Ts_nodes.Generic.t)
    : (runtime, scope, Node.node(Node.Constraint.any)) => {
  let is_sub =
    Path.hd(scope.path) |> CCOpt.map_or(~default=false, Path.is_sub);
  let identifiedNode = Ts_nodes_util.identifyGenericNode(node);
  switch (identifiedNode) {
  | StringKeyword(_)
  | NumberKeyword(_)
  | NeverKeyword(_)
  | NullKeyword(_)
  | ObjectKeyword(_)
  | UndefinedKeyword(_)
  | BooleanKeyword(_)
  | VoidKeyword(_)
  | AnyKeyword(_) => parse__Node__Basic(~runtime, ~scope, identifiedNode)
  | ArrayType(_) => parse__Node__Array(~runtime, ~scope, identifiedNode)
  | TupleType(_) => parse__Node__Tuple(~runtime, ~scope, identifiedNode)
  | TypeReference(_) =>
    parse__Node_TypeReference(~runtime, ~scope, identifiedNode)
  | TypeLiteral(_)
  | UnionType(_) when is_sub =>
    parse__Node__Generic__WrapSubNode(~runtime, ~scope, node)
  | TypeLiteral(typeLiteral) =>
    parse__Node__TypeLiteral(~runtime, ~scope, typeLiteral)
  | UnionType(union) => parse__Node__UnionType(~runtime, ~scope, union)
  | _ =>
    raise(
      Exceptions.UnexpectedAtThisPoint(
        Printf.sprintf(
          "> Cannot process generic node '%s'",
          node#getKindName(),
        ),
      ),
    )
  };
}
and parse__Node__Generic_assignable =
    (~runtime, ~scope, node: Ts_nodes.Generic.t)
    : (runtime, scope, Node.node(Node.Constraint.assignable)) => {
  let (runtime, scope, node) = parse__Node__Generic(~runtime, ~scope, node);
  (runtime, scope, node |> Node.Escape.toAssignable);
}
and parse__Node__Generic__WrapSubNode =
    (~runtime, ~scope, node: Ts_nodes.Generic.t) => {
  let name = Path.make_sub_type_name(scope.path);
  let type_name = Identifier.TypeName(name);
  let scope = scope |> Scope.add_to_path(type_name);
  let (runtime, scope, wrapped_type) =
    parse__Node__Generic_assignable(~runtime, ~scope, node);
  let wrapped_type_declaration =
    TypeDeclaration({
      path: scope.path,
      name: type_name,
      annot: wrapped_type,
      params: [||],
    });
  let scope = scope |> Scope.add_root_declaration(wrapped_type_declaration);
  (runtime, scope, Reference({target: [|type_name|], params: [||]}));
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Source File
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__SourceFile =
    (~runtime, ~scope, node: Ts_nodes.SourceFile.t)
    : (runtime, scope, Node.node(Node.Constraint.atLeastModule('poly))) => {
  let source_file_name = node#getBaseNameWithoutExtension();
  let scope = {...scope, source_file: Some(node)};
  let children_to_traverse: array(Ts_nodes.Generic.t) =
    runtime.parse_config.exports_only
      ? node#getExportedDeclarations() : node#getStatements();

  let (runtime: runtime, scope: scope) =
    CCArray.fold_left(
      ((runtime, scope), node) => {
        let scope = scope |> Scope.replace_path(Module(source_file_name));
        let (runtime, scope, res) =
          parse__Node__Declaration(~runtime, ~scope, node);
        let scope = scope |> Scope.add_root_declaration(res);
        (runtime, scope);
      },
      (runtime, scope),
      children_to_traverse,
    );

  let (runtime, scope, prependFixtures) =
    parse__Fixtures__ForSourceFile(~runtime, ~scope);
  (
    runtime,
    scope,
    Module({
      name: source_file_name,
      path: node#getFilePath(),
      types: CCArray.append(prependFixtures, scope.root_declarations),
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- TypeDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Declaration:
  (~runtime: runtime, ~scope: scope, Ts_nodes.Generic.t) =>
  (runtime, scope, Node.node(Node.Constraint.moduleLevel)) =
  (~runtime, ~scope, node: Ts_nodes.Generic.t) => {
    let identified_node = Ts_nodes_util.identifyNode(node);
    switch (identified_node) {
    // | ClassDeclaration
    // | InterfaceDeclaration
    | EnumDeclaration(enum) =>
      parse__Node__EnumDeclaration(~runtime, ~scope, enum)
    | FunctionDeclaration(fn_declaration) =>
      parse__Node__FunctionDeclaration(~runtime, ~scope, fn_declaration)
    // | VariableDeclaration
    | TypeAliasDeclaration(typeAlias) =>
      parse__Node__TypeAliasDeclaration(~runtime, ~scope, typeAlias)
    // | NamespaceDeclaration
    // | Expression
    | _ =>
      Console.error("> " ++ node#getKindName());
      raise(Failure("Only declarations allowed here"));
    };
  }
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- UnionType
//
// As the AST should stay as pure as possible, we need to construct the different versions of a union here directly
// Important for the union type:
// Whenever there is a Fixture(TUnboxed) present as the first child of a module, it will be printed as an unboxed module type
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__UnionType__Nodes:
  (
    ~runtime: runtime,
    ~scope: scope,
    array(Node.node(Node.Constraint.assignable))
  ) =>
  (runtime, scope, Node.node(Node.Constraint.assignable)) =
  (~runtime, ~scope, nodes) => {
    let union_type = Parser_union.determine_union_type(nodes);
    switch (union_type) {
    | Some(Optional(rest)) =>
      let (runtime, scope, t) =
        parse__Node__UnionType__Nodes(~runtime, ~scope, rest);
      (runtime, scope, Optional(t));
    | Some(Nullable(rest)) =>
      let (runtime, scope, t) =
        parse__Node__UnionType__Nodes(~runtime, ~scope, rest);
      (runtime, scope, Nullable(t));

    | Some(Single(one)) => (runtime, scope, one)
    | Some(Union(members)) =>
      let (runtime, scope, reference) =
        Parser_union.generate_ast_for_union(~runtime, ~scope, members);

      (runtime, scope, reference);
    | _ => raise(Not_found)
    };
  }
and parse__Node__UnionType = (~runtime, ~scope, node: Ts_nodes.UnionType.t) => {
  let type_nodes = node#getTypeNodes();
  let base_path = scope.path;
  let (runtime, scope, parsed_nodes) =
    type_nodes
    |> CCArray.foldi(
         ((runtime, scope, nodes), i, node) => {
           let current_path =
             base_path |> Path.add(Identifier.SubIdent(i + 1));
           let scope = scope |> Scope.replace_path_arr(current_path);
           let (runtime, scope, t) =
             parse__Node__Generic_assignable(~runtime, ~scope, node);
           (runtime, scope, CCArray.append(nodes, [|t|]));
         },
         (runtime, scope, [||]),
       );
  let scope = scope |> Scope.replace_path_arr(base_path);

  let (runtime, scope, t) =
    parse__Node__UnionType__Nodes(~runtime, ~scope, parsed_nodes);
  (runtime, scope, t |> Node.Escape.toAny);
}
// --- Any Helper
and parse__AssignAny = (~runtime, ~scope) => {
  (runtime, {...scope, has_any: true}, Basic(Any));
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- FunctionDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__FunctionDeclaration =
    (~runtime, ~scope, node: Ts_nodes.FunctionDeclaration.t) => {
  let name =
    switch (node#getName()) {
    | Some(name) => name
    | None when node#isDefaultExport() => "default"
    | None => raise(Failure("Expected a name for this function"))
    };
  let type_name: Identifier.t(Identifier.Constraint.exactlyTypeName) =
    Identifier.TypeName(name);
  let type_path = scope.path |> Path.add(Identifier.TypeName(name));
  let scope = {...scope, parent: Some(FunctionDeclaration(node))};

  let (runtime, scope, return_type) =
    switch (node#getReturnTypeNode()) {
    | Some(return_node) =>
      let scope =
        scope
        |> Scope.replace_path_arr(
             type_path |> Path.add(Identifier.SubName("return")),
           );
      parse__Node__Generic_assignable(~runtime, ~scope, return_node);
    | None => parse__AssignAny(~runtime, ~scope)
    };
  let scope = scope |> Scope.replace_path_arr(type_path);

  let (runtime, scope, parameters) =
    node#getParameters()
    |> CCArray.foldi(
         ((runtime, scope, params), i, param) => {
           let name = Identifier.PropertyName(param#getName());
           let is_optional = param#isOptional();
           let current_path =
             type_path |> Path.add(Identifier.SubName(Path.unwrap(name)));
           let scope = scope |> Scope.replace_path_arr(current_path);
           // TODO: isRestParameter
           let (runtime, scope, type_) =
             switch (param#getTypeNode()) {
             | None => parse__AssignAny(~runtime, ~scope)
             | Some(t) => parse__Node__Generic_assignable(~runtime, ~scope, t)
             };
           (
             runtime,
             scope,
             CCArray.append(
               params,
               [|Parameter({name, is_optional, type_, named: is_optional})|],
             ),
           );
         },
         (runtime, scope, [||]),
       );

  (
    runtime,
    scope,
    TypeDeclaration({
      path: scope.path,
      name: type_name,
      annot: Function({return_type, parameters}),
      params: [||],
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Type Literal
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__TypeLiteral =
    (~runtime, ~scope, node: Ts_nodes.TypeLiteral.t) => {
  let nodes_to_parse =
    node#getMembers() |> CCArray.map(Ts_nodes_util.identifyGenericNode);
  let (runtime, signatures) =
    nodes_to_parse
    |> CCArray.fold_left(
         ((runtime, nodes), node) => {
           let (runtime, _, signature) =
             parse__Node__SignatureLike(~runtime, ~scope, node);
           (
             runtime,
             switch (signature) {
             | (None, _, _) =>
               raise(
                 Failure("Type literal property should probably have a name"),
               )
             | (Some(name), is_optional, t) =>
               CCArray.append(
                 nodes,
                 [|
                   Parameter({
                     name: Identifier.PropertyName(name),
                     is_optional,
                     type_: t,
                     named: true,
                   }),
                 |],
               )
             },
           );
         },
         (runtime, [||]),
       );

  (runtime, scope, Record(signatures));
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Signature Hub
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__SignatureLike:
  (~runtime: runtime, ~scope: scope, Ts_nodes.nodeKind) =>
  (
    runtime,
    scope,
    (option(string), bool, Node.node(Node.Constraint.assignable)),
  ) =
  (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
    switch (node) {
    | PropertySignature(node) =>
      let name = Some(node#getName());
      let type_node = node#getTypeNode();
      let (runtime, _, t) =
        switch (type_node) {
        | None => parse__AssignAny(~runtime, ~scope)
        | Some(type_node) =>
          parse__Node__Generic_assignable(
            ~runtime,
            ~scope=
              scope |> Scope.add_to_path(Identifier.SubName(node#getName())),
            type_node,
          )
        };
      let is_optional = node#getQuestionTokenNode() |> CCOpt.is_some;
      (runtime, scope, (name, is_optional, t));
    | MethodSignature(_)
    | ConstructSignatureDeclaration(_)
    | CallSignatureDeclaration(_)
    | IndexSignatureDeclaration(_) => raise(Not_found)
    | _ => raise(Failure("Not an signature"))
    };
  }
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- EnumDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__EnumDeclaration =
    (~runtime, ~scope, node: Ts_nodes.EnumDeclaration.t) => {
  // TODO: Solving this simply for now, we need to look at the propertyName eventuall though!!
  let members = node#getMembers();
  let type_name: Identifier.t(Identifier.Constraint.exactlyTypeName) =
    Identifier.TypeName(node#getName());
  let scope = {
    ...scope,
    parent: Some(EnumDeclaration(node)),
    path:
      CCArray.append(scope.path, [|Identifier.TypeName(node#getName())|]),
  };
  let variant_constructors =
    members
    |> CCArray.map(member =>
         {
           VariantConstructor.name:
             Identifier.VariantIdentifier(member#getName()),
           arguments: [||],
         }
       );
  let annotation = Variant(variant_constructors);

  (
    runtime,
    scope,
    TypeDeclaration({
      path: scope.path,
      name: type_name,
      annot: annotation,
      params: [||],
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- TypeAliasDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__TypeAliasDeclaration:
  (~runtime: runtime, ~scope: scope, Ts_nodes.TypeAliasDeclaration.t) =>
  (runtime, scope, Node.node(Node.Constraint.moduleLevel)) =
  (~runtime, ~scope, node: Ts_nodes.TypeAliasDeclaration.t) => {
    let type_name: Identifier.t(Identifier.Constraint.exactlyTypeName) =
      Identifier.TypeName(node#getName());
    let scope = {
      ...scope,
      parent: Some(TypeAliasDeclaration(node)),
      path:
        CCArray.append(scope.path, [|Identifier.TypeName(node#getName())|]),
    };

    let (runtime, scope, annotation) =
      parse__Node__Generic_assignable(~runtime, ~scope, node#getTypeNode());

    (
      runtime,
      scope,
      TypeDeclaration({
        path: scope.path,
        name: type_name,
        annot: annotation,
        params: [||],
      }),
    );
  }
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Basic Nodes
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Basic = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | StringKeyword(_) => (runtime, scope, Basic(String))
  | NumberKeyword(_) => (runtime, scope, Basic(Number))
  | NeverKeyword(_) => (runtime, scope, Basic(Never))
  | ObjectKeyword(_) => (runtime, scope, Basic(RelevantKeyword("Object")))
  | UndefinedKeyword(_) => (runtime, scope, Basic(Undefined))
  | VoidKeyword(_) => (runtime, scope, Basic(Void))
  | NullKeyword(_) => (runtime, scope, Basic(Null))
  | BooleanKeyword(_) => (runtime, scope, Basic(Boolean))
  | AnyKeyword(_) => (runtime, {...scope, has_any: true}, Basic(Any))
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not a basic type"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Tuples
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Tuple = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | TupleType(node) =>
    // TODO: Handle potentially extracted type here
    // Whenever "inner" types are parsed, we should use a separate "parse__Node" function
    // Maybe

    let children_to_traverse = node#getElementTypeNodes();
    let (
      runtime: runtime,
      scope: scope,
      inner: array(Node.node(Node.Constraint.assignable)),
    ) =
      CCArray.fold_left(
        ((runtime, scope, nodes), node) => {
          let (runtime, scope, res) =
            parse__Node__Generic_assignable(~runtime, ~scope, node);
          (runtime, scope, CCArray.append(nodes, [|res|]));
        },
        (runtime, scope, [||]),
        children_to_traverse,
      );

    (runtime, scope, Tuple(inner));
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not a tuple"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- TypeReference
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node_TypeReference = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | TypeReference(node) =>
    node#getTypeArguments() |> CCArray.iter(n => Console.log(n#getKindName()));

    (
      runtime,
      scope,
      Reference({
        target: [|Identifier.TypeName(node#getTypeName()#getText())|],
        params: [||],
      }),
    );
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not a type reference"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Arrays
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Array = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | ArrayType(node) =>
    let (runtime, scope, inner) =
      // TODO: Handle potentially extracted type here
      // Whenever "inner" types are parsed, we should use a separate "parse__Node" function
      // Maybe
      parse__Node__Generic_assignable(
        ~runtime,
        ~scope,
        node#getElementTypeNode(),
      );
    (runtime, scope, Array(inner));

  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not an array"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Adding Fixtures
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Fixtures__ForSourceFile = (~runtime, ~scope) => {
  (
    runtime,
    scope,
    if (scope.has_any) {
      [|Fixture(AnyUnboxed)|];
    } else {
      [||];
    },
  );
};
