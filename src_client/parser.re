// TODO: Eventually define an interface so that multiple backends can be built
// ALso: Plugin system should be solved here as well, before the AST is built
// Could use a similar system to graphql-ppx (see https://github.com/reasonml-community/graphql-ppx/blob/master/src/base/validations.re for reference)
open Ast;
open Node;
module Exceptions = {
  exception UnexpectedAtThisPoint(string);
};

// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Mutable structures for referencing
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
type runtime = {
  order: node_order,
  references: node_references,
  node_count: int,
  exports_only: bool,
};
type scope = {
  source_file: option(Ts_nodes.SourceFile.t),
  parent: option(Ts_nodes.nodeKind),
  path: Identifier.path,
  has_any: bool,
};

// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Entry / Preparation
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
let rec parse__Entry = (~source_files: array(Ts_morph.SourceFile.t)) => {
  source_files
  |> CCArray.map(source_file => {
       // These variables are individual for each source file for the time being
       let runtime = {
         order: [||],
         references: Hashtbl.create(0),
         node_count: 0,
         exports_only: false,
       };
       let scope = {
         source_file: None,
         parent: None,
         path: [||],
         has_any: false,
       };

       let source_file =
         source_file
         |> Ts_morph.SourceFile.castToNode
         |> Ts_nodes.Generic.fromMorphNode;
       parse__Node__Generic(~runtime, ~scope, source_file);
     });
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Generic Node ("Hub")
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Generic =
    (~runtime, ~scope, node: Ts_nodes.Generic.t)
    : (runtime, scope, Node.node(Node.Constraint.any)) => {
  let identifiedNode = Ts_nodes_util.identifyGenericNode(node);
  switch (identifiedNode) {
  | SourceFile(sourceFile) =>
    parse__Node__SourceFile(~runtime, ~scope, sourceFile)
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
  | _ =>
    Console.error("> " ++ node#getKindName());
    raise(Failure("OH no"));
  };
}
and parse__Node__Generic_assignable =
    (~runtime, ~scope, node: Ts_nodes.Generic.t)
    : (runtime, scope, Node.node(Node.Constraint.assignable)) => {
  let (runtime, scope, node) = parse__Node__Generic(~runtime, ~scope, node);
  (runtime, scope, node |> Node.Escape.toAssignable);
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Source File
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__SourceFile =
    (~runtime, ~scope, node: Ts_nodes.SourceFile.t)
    : (runtime, scope, Node.node(Node.Constraint.atLeastSourceFile('poly))) => {
  let source_file_name = node#getBaseNameWithoutExtension();
  let scope = {
    ...scope,
    source_file: Some(node),
    path: [|Identifier.Module(source_file_name)|],
  };
  let children_to_traverse: array(Ts_nodes.Generic.t) =
    runtime.exports_only
      ? node#getExportedDeclarations() : node#getStatements();
  let (
    runtime: runtime,
    scope: scope,
    types: array(Node.node(Node.Constraint.moduleLevel)),
  ) =
    CCArray.fold_left(
      ((runtime, scope, nodes), node) => {
        let (runtime, scope, res) =
          parse__Node__Declaration(~runtime, ~scope, node);
        (runtime, scope, CCArray.append([|res|], nodes));
      },
      (runtime, scope, [||]),
      children_to_traverse,
    );

  let (runtime, scope, prependFixtures) =
    parse__Fixtures__ForSourceFile(~runtime, ~scope);
  (
    runtime,
    scope,
    SourceFile({
      name: source_file_name,
      path: node#getFilePath(),
      types: CCArray.append(prependFixtures, types),
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- TypeDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Declaration:
  (~runtime: runtime, ~scope: scope, Ts_morph.Node.t) =>
  (runtime, scope, Node.node(Node.Constraint.moduleLevel)) =
  (~runtime, ~scope, node: Ts_morph.Node.t) => {
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
  let scope = {
    ...scope,
    parent: Some(FunctionDeclaration(node)),
    path: scope.path |> CCArray.append([|Identifier.TypeName(name)|]),
  };

  let (runtime, scope, return_type) =
    switch (node#getReturnTypeNode()) {
    | Some(return_node) =>
      parse__Node__Generic_assignable(~runtime, ~scope, return_node)
    | None => (runtime, scope, Basic(Any))
    };

  let (runtime, scope, parameters) =
    node#getParameters()
    |> CCArray.fold_left(
         ((runtime, scope, params), param) => {
           let name = Identifier.PropertyName(param#getName());
           let is_optional = param#isOptional();
           // TODO: isRestParameter
           let (runtime, scope, type_) =
             switch (param#getTypeNode()) {
             | None => (runtime, scope, Basic(Any))
             | Some(t) => parse__Node__Generic_assignable(~runtime, ~scope, t)
             };
           (
             runtime,
             scope,
             CCArray.append(
               [|Parameter({name, is_optional, type_, named: is_optional})|],
               params,
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
      extracted_nodes: [||],
      name: type_name,
      annot: Function({return_type, parameters}),
      params: [||],
    }),
  );
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
      scope.path |> CCArray.append([|Identifier.TypeName(node#getName())|]),
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
      extracted_nodes: [||],
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
        scope.path |> CCArray.append([|Identifier.TypeName(node#getName())|]),
    };

    let (runtime, scope, annotation) =
      parse__Node__Generic_assignable(~runtime, ~scope, node#getTypeNode());

    (
      runtime,
      scope,
      TypeDeclaration({
        path: scope.path,
        extracted_nodes: [||],
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
          (runtime, scope, CCArray.append([|res|], nodes));
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
