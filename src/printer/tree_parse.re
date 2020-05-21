open Re_typescript_base;
open Tree_types;
open Tree_utils;

module Type = {
  type t = Hashtbl.t(Path.t, ts_type);
  let map: t = Hashtbl.create(0);
  let add = (~path, type_) => Hashtbl.add(map, path, type_);
  let get = (~path) => Hashtbl.find_opt(map, path);
  let replace = (~path, type_) => Hashtbl.replace(map, path, type_);

  let order = ref([]);
  let add_order = (path: Path.t) => {
    order := order^ @ [path];
  };
  let clear = () => {
    Hashtbl.clear(map);
    order := [];
  };
};
module Ref = {
  type t = Hashtbl.t(Path.t, Path.t);
  let map: t = Hashtbl.create(0);
  let add = (~from: Path.t, ~to_: Path.t) => Hashtbl.add(map, to_, from);
  let get = (to_: Path.t) => Hashtbl.find_opt(map, to_);
  let get_all = (to_: Path.t) => Hashtbl.find_all(map, to_);
  let clear = () => {
    Hashtbl.clear(map);
  };

  let resolve_ref = (~from: Path.t, ~lookup: list(string)): option(Path.t) => {
    let scope = from |> Path.to_scope;
    switch (lookup) {
    | [] => None
    | [_] as one =>
      let path = (scope @ one, []);
      add(~from, ~to_=path);
      Type.get(~path) |> CCOpt.map(_ => path);
    | _ => raise(Exceptions.Parser_error("PATH NOT IMPLEMENTED"))
    };
  };
};

/**
    Type / Interface / Enum definitions
 */
let rec parse__type_def =
        (~inline=false, ~path=([], []), type_def: Ts.type_def) => {
  switch (type_def) {
  | `TypeDef(name, type_, args) =>
    let ident = name |> Ident.of_string;
    let t_path = inline ? path : path |> Path.add_ident(ident);
    let t =
      TypeDeclaration({
        td_name: ident,
        td_type: parse__type(~path=t_path, type_),
        td_arguments: parse__type_args(~path=t_path, args),
      });
    Type.add_order(t_path);
    Type.add(~path=t_path, t);
  | `Module(m) => parse__module(~path, m)
  | `InterfaceDef(name, extends, fields, args) =>
    let ident = name |> Ident.of_string;
    let t_path = inline ? path : path |> Path.add_ident(ident);
    let t =
      TypeDeclaration({
        td_name: ident,
        td_type: parse__type(~path=t_path, `Obj(fields)),
        td_arguments: parse__type_args(~path=t_path, args),
      });
    Type.add_order(t_path);
    Type.add(~path=t_path, t);
  | `EnumDef(name, members, is_const) =>
    let ident = name |> Ident.of_string;
    let t_path = inline ? path : path |> Path.add_ident(ident);
    let t =
      TypeDeclaration({
        td_name: ident,
        td_type: parse__type(~path=t_path, `Enum((members, is_const))),
        td_arguments: [],
      });
    Type.add_order(t_path);
    Type.add(~path=t_path, t);
  };
}
/**
    Unions
 */
and parse__union = (~path, members) => {
  // TODO: Switch on references in the simple unions to maybe get the same type
  switch (parse__union_undefined(~path, members)) {
  | Some(t) => t
  | None =>
    switch (parse__union_nullable(~path, members)) {
    | Some(t) => t
    | None =>
      switch (parse__string_literal(members)) {
      | Some(t) => t
      | None =>
        switch (parse__numeric_literal(members)) {
        | Some(t) => t
        | None =>
          switch (parse__mixed_literal(members)) {
          | Some(t) => t
          | None => parse__union_type(~path, members)
          }
        }
      }
    }
  };
}
and parse__union_type = (~path, members: list(Ts.union_member)) => {
  let (strings, numbers, _, types) =
    members
    |> CCList.fold_left(
         ((str, num, bool, types)) =>
           fun
           | `U_String(_) as s => (str @ [s], num, bool, types)
           | `U_Number(_) as n => (str, num @ [n], bool, types)
           | `U_Bool(_) => (
               str,
               num,
               true,
               bool ? types : types @ [Base(Boolean)],
             )
           | `U_Type(n) => (
               str,
               num,
               bool,
               types @ [parse__type(~inline=true, ~path, n)],
             ),
         ([], [], false, []),
       );

  let types =
    types
    @ (
      strings
      |> list_to_opt
      |> CCOpt.flat_map(parse__string_literal)
      |> CCOpt.to_list
    )
    @ (
      numbers
      |> list_to_opt
      |> CCOpt.flat_map(parse__numeric_literal)
      |> CCOpt.to_list
    );
  Union(
    types
    |> CCList.map(t =>
         {um_type: t, um_classifier: "", um_ident: get_union_type_name(t)}
       ),
  );
}
and parse__mixed_literal = (members: list(Ts.union_member)) => {
  exception No_union_mixed;
  try(
    Some(
      MixedLiteral(
        members
        |> CCList.fold_left(
             p =>
               fun
               | `U_String(n) => {
                   ...p,
                   strings: p.strings @ [n |> Ident.of_string],
                 }
               | `U_Number(n) => {...p, numbers: p.numbers @ [n]}
               | `U_Bool(n) => {...p, bools: p.bools @ [n]}
               | _ => raise(No_union_mixed),
             {strings: [], numbers: [], bools: []},
           ),
      ),
    )
  ) {
  | No_union_mixed => None
  | e => raise(e)
  };
}
and parse__numeric_literal = (members: list(Ts.union_member)) => {
  exception No_union_number;
  try(
    Some(
      NumericLiteral(
        members
        |> CCListLabels.map(
             ~f=
               fun
               | `U_Number(n) => n
               | _ => raise(No_union_number),
           ),
      ),
    )
  ) {
  | No_union_number => None
  | e => raise(e)
  };
}
and parse__string_literal = (members: list(Ts.union_member)) => {
  exception No_union_string;
  try(
    Some(
      StringLiteral(
        members
        |> CCList.map(
             fun
             | `U_String(str) => str |> Ident.of_string
             | _ => raise(No_union_string),
           ),
      ),
    )
  ) {
  | No_union_string => None
  | e => raise(e)
  };
}
and parse__union_undefined = (~path, members: list(Ts.union_member)) => {
  let extract_undefined =
    members
    |> CCListLabels.fold_left(
         ~f=
           ((has_undefined, p), member) => {
             switch (member) {
             | `U_Type(`Undefined) => (true, p)
             | member => (has_undefined, [member, ...p])
             }
           },
         ~init=(false, []),
       );
  switch (extract_undefined) {
  | (true, [`U_Type(type_)]) => Some(Optional(parse__type(~path, type_)))
  | (true, members) => Some(Optional(parse__union(~path, members)))
  | (false, _) => None
  };
}
and parse__union_nullable = (~path, members: list(Ts.union_member)) => {
  let extract_null =
    members
    |> CCListLabels.fold_left(
         ~f=
           ((has_null, p), member) => {
             switch (member) {
             | `U_Type(`Null) => (true, p)
             | member => (has_null, [member, ...p])
             }
           },
         ~init=(false, []),
       );
  switch (extract_null) {
  | (true, [`U_Type(type_)]) => Some(Nullable(parse__type(~path, type_)))
  | (true, members) => Some(Nullable(parse__union(~path, members)))
  | (false, _) => None
  };
}
/**
    Type arguments
 */
and parse__type_args = (~path, args: list(Ts.type_arg)) => {
  args
  |> CCList.map(({a_name, a_constraint_, a_default}: Ts.type_arg) =>
       (
         {
           let ident = a_name |> Ident.of_string;
           let arg_path =
             Path.(path |> add_sub("arg") |> add_sub_ident(ident));
           {
             tda_name: ident,
             tda_extends:
               a_constraint_ |> CCOpt.map(parse__type(~path=arg_path)),
             tda_default:
               a_default |> CCOpt.map(parse__type(~path=arg_path)),
           };
         }: ts_type_argument
       )
     );
}
/**
    Type extraction
 */
and parse__type_extraction = (~path, type_ref: Ts.ref_, names) => {
  Base(Any);
}
/**
    Array
 */
and parse__array = (~path, type_) => {
  Array(parse__type(~inline=true, ~path=path |> Path.add_sub("t"), type_));
}
/**
    Type reference
 */
and parse__type_reference = (~path, type_ref: Ts.ref_) => {
  let (ref_path, ref_types) = CCList.split(type_ref);

  Reference({
    tr_path: ref_path,
    tr_path_resolved: Ref.resolve_ref(~from=path, ~lookup=ref_path),
    tr_parameters:
      ref_types
      |> CCList.last_opt
      |> CCOpt.map_or(~default=[], CCList.map(parse__type(~path))),
  });
}
/**
    Enums
 */
and parse__enum = (~path, members: list(Ts.enum_field), is_const: bool) => {
  let is_clean =
    members
    |> CCList.for_all((member: Ts.enum_field) =>
         !(member.Ts.default |> CCOpt.is_some)
       );
  if (is_clean) {
    Enum(
      members
      |> CCList.map((member: Ts.enum_field) => member.key)
      |> CCList.map(Ident.of_string),
    );
  } else {
    raise(Exceptions.Parser_error("Unclean enums are not yet supported"));
  };
}
/**
    Tuples
 */
and parse__tuple = (~path, types) => {
  Tuple(
    types
    |> CCList.mapi((i, type_) => {
         let path = path |> Path.add_sub(string_of_int(i + 1));
         parse__type(~inline=true, ~path, type_);
       }),
  );
}
/**
    Interfaces
 */
and parse__interface = (~path, fields: list(Ts.obj_field)) => {
  Interface(
    fields
    |> CCList.map(({f_key, f_type_, f_optional, f_readonly}: Ts.obj_field) => {
         let path = path |> Path.add_sub(f_key);
         let parsed_type = parse__type(~inline=true, ~path, f_type_);
         {
           f_name: f_key |> Ident.of_string,
           f_type: f_optional ? Optional(parsed_type) : parsed_type,
           f_readonly,
         };
       }),
  );
}
/**
    All types
 */
and parse__type = (~inline=false, ~path, type_: Ts.type_) => {
  switch (type_) {
  | `String => Base(String)
  | `Number => Base(Number)
  | `Boolean => Base(Boolean)
  | `Void => Base(Void)
  | `Any => Base(Any)
  | `Null => Base(Null)
  | `Undefined => Base(Undefined)
  | `Enum(members, is_const) => parse__enum(~path, members, is_const)
  | `Union(members) => parse__union(~path, members)
  | `Tuple(types) => parse__tuple(~path, types)
  | `Array(t) => parse__array(~path, t)
  | `Ref(ref_) => parse__type_reference(~path, ref_)
  | `TypeExtract(type_ref, names) =>
    parse__type_extraction(~path, type_ref, names)
  | `Obj(fields) as t =>
    inline
      ? {
        parse__inline(~path, t);
      }
      : parse__interface(~path, fields)
  };
}
and parse__inline = (~path, type_) => {
  parse__type_def(~inline=true, ~path, `TypeDef(("", type_, [])));
  Reference({
    tr_path: fst(path),
    tr_path_resolved: Some(path),
    tr_parameters: [],
  });
}
/**
    Module
 */
and parse__module = (~path, module_: Ts.module_) => {
  module_.types
  |> CCList.iter(parse__type_def(~path=path |> Path.add(module_.name)));
}
/**
    Entry module
 */
and parse__entry_module = (module_: Ts.module_) => {
  Type.clear();
  Ref.clear();
  parse__type_def(`Module({...module_, name: ""}));

  Type.order^
  |> CCList.iteri((i, path) => {
       switch (Type.get(~path)) {
       | Some(TypeDeclaration({td_type: Interface(fields), _} as td))
           when fields |> CCList.length == 0 =>
         if (Ref.get_all(path) |> CCList.length == 0) {
           Type.order := Type.order^ |> CCList.remove_one(~eq=Path.eq, path);
         } else {
           Type.replace(
             ~path,
             TypeDeclaration({...td, td_type: Base(Any)}),
           );
         }
       | _ => ()
       }
     });

  (Type.order^, Type.map);
};
