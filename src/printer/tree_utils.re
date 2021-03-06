open Re_typescript_base;
open String_utils;
open Tree_types;

module Ident = {
  type t = ts_identifier;

  let of_string = (str: string): t => {
    i_value: str,
    i_ident: str |> to_valid_ident,
    i_type: str |> to_valid_typename,
    i_module: str |> to_valid_module_name,
    i_variant: str |> to_valid_variant,
  };
  let of_pi = ({item, _}: Re_typescript_base.Ts.with_pi(string)) =>
    of_string(item);
  let of_int = (i: int): t => {
    let str = string_of_int(i);
    {
      i_value: str,
      i_ident: str |> to_valid_ident,
      i_type: str |> to_valid_typename,
      i_module: str |> to_valid_module_name,
      i_variant: str |> to_valid_variant,
    };
  };
  let value = ({i_value, _}: t) => i_value;
  let ident = ({i_ident, _}: t) => i_ident;
  let type_ = ({i_type, _}: t) => i_type;
  let module_ = ({i_module, _}: t) => i_module;
  let variant = ({i_variant, _}: t) => i_variant;
  let eq = (i1: t, i2: t) => CCEqual.string(i1 |> ident, i2 |> ident);
};

module Path = {
  type t = ts_path;
  let add = (str, (path, sub): t): t =>
    str != "" ? (path @ [str], sub) : (path, sub);
  let add_sub = (str, (path, sub): t): t => (path, sub @ [str]);
  let add_ident = (ident, (path, sub): t): t => (
    path @ [ident |> Ident.value],
    sub,
  );
  let add_sub_ident = (ident, (path, sub): t): t => (
    path,
    sub @ [ident |> Ident.value],
  );
  let add_ident = (ident, (path, sub): t): t => (
    path @ [ident |> Ident.value],
    sub,
  );
  let cut_unscoped = l =>
    switch (l) {
    | [] => []
    | [_] => []
    | l => l |> CCList.remove_at_idx(CCList.length(l) - 1)
    };
  let cut_unscoped_keep = l =>
    switch (l) {
    | [] => (None, [])
    | [one] => (Some(one), [])
    | l =>
      let i = CCList.length(l) - 1;
      (l |> CCList.get_at_idx(i), l |> CCList.remove_at_idx(i));
    };
  let cut_sub = ((path, sub)) => (path, cut_unscoped(sub));
  let cut = ((path, sub)) => (cut_unscoped(path), sub);
  let no_sub = ((path, sub)) => (path, []);
  let has_sub = ((_, sub): t) => sub |> CCList.length > 0;
  let base = ((path, _)) => path;
  let sub = ((_, sub)) => sub;
  let to_typename = ((path, sub): t): string =>
    path
    @ sub
    |> CCList.to_string(~sep="_", v => v)
    |> Ident.of_string
    |> Ident.type_;
  let to_scope = ((path, _): t): list(string) =>
    switch (path |> CCList.rev) {
    | []
    | [_] => []
    | [_, ...r] => r |> CCList.rev
    };
  let to_full_path = ((path, sub): t) => path @ sub;
  let to_unscoped_path = ((path, sub): t) => (path |> CCList.last(1)) @ sub;
  let unscoped_to_string = CCList.to_string(~sep="_", a => a);
  let to_string = ((path, sub): t) => path @ sub |> unscoped_to_string;
  let to_ident = path => path |> to_string |> Ident.of_string;
  let eq_unscoped = CCEqual.list(CCEqual.string);
  let eq = ((path_a, sub_a): t, (path_b, sub_b): t) => {
    eq_unscoped(path_a, path_b) && eq_unscoped(sub_a, sub_b);
  };

  let pp_unscoped = (~sep=".", sub: list(string)) =>
    sub |> CCList.to_string(~sep, a => a);
  let pp = (~sep=?, p) =>
    switch (p) {
    | (path, []) => Printf.sprintf("'%s'", pp_unscoped(~sep?, path))
    | (path, sub) =>
      Printf.sprintf(
        "'%s -> %s'",
        pp_unscoped(~sep?, path),
        pp_unscoped(~sep?, sub),
      )
    };
};

module Exceptions = {
  exception Parser_unexpected(string);
  exception Parser_error(string);
  exception Parser_parameter_error(string);
  exception Parser_unsupported(string, Parse_info.t);
  exception Optimizer_error(string);
  exception File_error(string);
  exception Needs_lazy;
};

let replace_ref_in_union_members =
    (~sub: Path.t, ~by: Path.t, ~parameters, members: list(ts_union_member)) => {
  let rec run = (t: ts_type) =>
    switch (t) {
    | Reference({tr_path_resolved: Some(tr_path), _} as rf)
        when Path.eq(tr_path, sub) =>
      Reference({
        ...rf,
        tr_path_resolved: Some(by),
        tr_parameters: parameters |> CCList.map(param => Arg(param)),
      })
    | Array(t) => Array(run(t))
    | Nullable(t) => Nullable(run(t))
    | Optional(t) => Optional(run(t))
    | Interface(fields, b) =>
      Interface(
        fields |> CCList.map(field => {...field, f_type: run(field.f_type)}),
        b,
      )
    | other => other
    };
  members
  |> CCList.map(({um_type, _} as um) => {...um, um_type: run(um_type)});
};

// let has_ref_or_arg_from_ident = (~ident: Ident.t, ty: Ts.type_) => {
//   let rec run = (t: Ts.type_) => switch(t) {
//     | Array(t) => Array(run(t))

//   };
// };

let rec get_union_type_name = (um_type: ts_type) => {
  switch (um_type) {
  | Base(String) => "string"
  | Base(Number) => "number"
  | Base(Boolean) => "boolean"
  | Base(Void) => "void"
  | Base(Null) => "null"
  | Base(Undefined) => "undefined"
  | Base(Any) => "any"
  | Function(_) => "func"
  | Reference({tr_path_resolved, _}) =>
    tr_path_resolved
    |> CCOpt.flat_map(((path, sub)) =>
         CCList.last_opt(path)
         |> CCOpt.map(l => [l] @ sub |> CCList.to_string(~sep="_", a => a))
       )
    |> CCOpt.value(~default="unknown")
    |> Ident.of_string
    |> Ident.ident
  | Optional(t)
  | Nullable(t) => get_union_type_name(t)
  | Array(t) => Printf.sprintf("arr_%s", get_union_type_name(t))
  | Tuple(_) => "tuple"
  | MixedLiteral(_) => "variant"
  | NumericLiteral(_) => "num"
  | StringLiteral(_) => "literal"
  | Enum(_) => "enum"
  | Arg(i) => i |> Ident.ident
  | Interface(_) => "interface"
  | Base(Never) =>
    raise(Exceptions.Parser_error("Never is not a valid union member"))
  | Union(_) =>
    raise(Exceptions.Parser_error("Union is not a valid union member"))
  | TypeDeclaration(_) =>
    raise(
      Exceptions.Parser_error("TypeDeclaration is not a valid union member"),
    )
  | Import(_) =>
    raise(Exceptions.Parser_error("Import is not a valid union member"))
  | Module(_) =>
    raise(Exceptions.Parser_error("Module is not a valid union member"))
  | Lazy(_) =>
    raise(Exceptions.Parser_error("Lazy is not a valid union member"))
  | ResolveWithParams(_) =>
    raise(
      Exceptions.Parser_error(
        "ResolveWithParams is not a valid union member",
      ),
    )
  };
};

let rec path_of_declaration = (~path, d: Ts.declaration) =>
  switch (d) {
  | Type({item: {t_ident: {item, _}, _}, _})
  | Interface({item: {i_ident: {item, _}, _}, _})
  | Enum({item: {e_ident: {item, _}, _}, _})
  | FunctionDec({item: {f_ident: {item, _}, _}, _})
  | Namespace({item: {n_ident: [{item, _}], _}, _})
  // | Module({item: (item, _), _})
  | Class({item: {c_ident: Some({item, _}), _}, _}) =>
    Some(path |> Path.add(item))
  | Variable(_) =>
    // TODO: Need to be split up before reaching this point
    None
  | Ambient(d)
  | Export(d)
  | ExportDefault(d) => path_of_declaration(~path, d)
  | Module(_)
  | Namespace(_)
  | Class(_)
  | ExportList(_)
  | ExportAssignment(_)
  | ImportAlias(_)
  | IdentifierReference(_)
  | Import(_) => None
  };

let rec type_to_string = (t: Ts.type_) =>
  switch (t) {
  | Object(_) => "Object"
  | MappedObject({item: {mo_type, _}, _}) =>
    Printf.sprintf("MappedObject_%s", type_to_string(mo_type))
  | Tuple(_) => "Tuple"
  | Array(_) => "Array"
  | Function(_) => "Function"
  | Constructor(_) => "Constructor"
  | Intersection(_) => "Intersection"
  | Union(_) => "Union"
  | Query(_) => "Query"
  | KeyOf(_) => "KeyOf"
  | StringLiteral(_) => "StringLiteral"
  | NumberLiteral(_) => "NumberLiteral"
  | BoolLiteral(_) => "BoolLiteral"
  | Never(_) => "Never"
  | Conditional(_) => "Conditional"
  | TypeReference((path, _)) =>
    Printf.sprintf(
      "TypeReference: %s",
      path |> CCList.to_string(~sep=".", p => p.Ts.item),
    )
  | TypeExtract(_) => "TypeExtract"
  | String(_) => "String"
  | Number(_) => "Number"
  | Boolean(_) => "Boolean"
  | Null(_) => "Null"
  | Undefined(_) => "Undefined"
  | Void(_) => "Void"
  | Any(_) => "Any"
  | Symbol(_) => "Symbol"
  | This(_) => "This"
  | UnionTemp(_) => "UnionTemp"
  };

let rec declaration_to_string = (d: Ts.declaration) =>
  switch (d) {
  | IdentifierReference(_) => "IdentifierReference"
  | ExportList(_) => "ExportList"
  | Export(d) => Printf.sprintf("Export_%s", declaration_to_string(d))
  | ExportAssignment(_) => "ExportAssignment"
  | ExportDefault(d) => Printf.sprintf("Export_%s", declaration_to_string(d))
  | Ambient(d) => Printf.sprintf("Export_%s", declaration_to_string(d))
  | Namespace(_) => "Namespace"
  | Module(_) => "Module"
  | Type(_) => "Type"
  | Interface(_) => "Interface"
  | Enum(_) => "Enum"
  | FunctionDec(_) => "FunctionDec"
  | Class(_) => "Class"
  | Variable(_) => "Variable"
  | ImportAlias(_) => "ImportAlias"
  | Import({item: {i_clause, i_from}, _}) =>
    switch (i_clause) {
    | TripleSlashReference =>
      Printf.sprintf("TripleSlashReference (%s)", i_from)
    | ImportModuleSpecifier =>
      Printf.sprintf("ImportModuleSpecifier (%s)", i_from)
    | ImportBinding(_) => Printf.sprintf("ImportBinding (%s)", i_from)
    | ImportNamespace(_) => Printf.sprintf("ImportNamespace (%s)", i_from)
    | ImportNamed(_) => Printf.sprintf("ImportNamed (%s)", i_from)
    | ImportSplitNamespace(_) =>
      Printf.sprintf("ImportSplitNamespace (%s)", i_from)
    | ImportSplitNamed(_) => Printf.sprintf("ImportSplitNamed (%s)", i_from)
    }
  };

let rec position_of_declaration: Ts.declaration => Parse_info.t =
  fun
  | IdentifierReference({pi, _}) => pi
  | Export(d) => position_of_declaration(d)
  | ExportDefault(d) => position_of_declaration(d)
  | Ambient(d) => position_of_declaration(d)
  | ExportAssignment({pi, _}) => pi
  | ExportList({pi, _}) => pi
  | Namespace({pi, _}) => pi
  | Module({pi, _}) => pi
  | Type({pi, _}) => pi
  | Interface({pi, _}) => pi
  | Enum({pi, _}) => pi
  | FunctionDec({pi, _}) => pi
  | Class({pi, _}) => pi
  | Variable({pi, _}) => pi
  | ImportAlias({pi, _}) => pi
  | Import({pi, _}) => pi;

let position_of_property_name: Ts.property_name => Parse_info.t =
  fun
  | PIdentifier({pi, _}) => pi
  | PString({pi, _}) => pi
  | PNumber({pi, _}) => pi;

let position_of_type_member: Ts.type_member => Parse_info.t =
  fun
  | PropertySignature({ps_property_name, _}) =>
    ps_property_name |> position_of_property_name
  | MethodSignature({ms_property_name, _}) =>
    ms_property_name |> position_of_property_name
  | CallSignature(_) => Parse_info.zero
  | ConstructSignature(_) => Parse_info.zero
  | IndexSignature({is_ident: {pi, _}, _}) => pi;

let rec ts_to_string = (t: ts_type) =>
  switch (t) {
  | Function(_) => "Function"
  | Union(_) => "Union"
  | MixedLiteral(_) => "MixedLiteral"
  | NumericLiteral(_) => "NumericLiteral"
  | StringLiteral(lst) =>
    Printf.sprintf(
      "StringLiteral: %s",
      lst |> CCList.to_string(~sep=",", Ident.value),
    )
  | Enum(_) => "Enum"
  | Base(String) => "Base_String"
  | Base(Number) => "Base_Number"
  | Base(Boolean) => "Base_Boolean"
  | Base(Void) => "Base_Void"
  | Base(Any) => "Base_Any"
  | Base(Null) => "Base_Null"
  | Base(Never) => "Base_Never"
  | Base(Undefined) => "Base_Undefined"
  | Interface(f, extended) =>
    Printf.sprintf(
      "Interface (%i, extended: %s, fields: %s)",
      f |> CCList.length,
      extended ? "true" : "false",
      f
      |> CCList.to_string(~sep=" | ", f =>
           Printf.sprintf(
             "(%s: %s)",
             f.f_name.i_value,
             ts_to_string(f.f_type),
           )
         ),
    )
  | Tuple(_) => "Tuple"
  | Array(_) => "Array"
  | Optional(_) => "Optional"
  | Nullable(_) => "Nullable"
  | Reference({tr_path, tr_path_resolved, tr_parameters}) =>
    Printf.sprintf(
      "Reference (resolved: %s, parameters: %s)",
      Path.pp(tr_path_resolved |> CCOpt.get_exn),
      ts_lst_to_string(tr_parameters),
    )
  | TypeDeclaration({td_type, _}) =>
    Printf.sprintf("TypeDeclaration_%s", ts_to_string(td_type))
  | Import(_) => "Import"
  | Module(_) => "Module"
  | Lazy(_) => "Lazy"
  | ResolveWithParams(_) => "ResolveWithParams"
  | Arg(i) => Printf.sprintf("Arg: %s", Ident.value(i))
  }
and ts_lst_to_string = lst => CCList.to_string(~sep=", ", ts_to_string, lst);
