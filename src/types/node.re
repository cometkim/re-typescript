[@ocaml.warning "-30"];

module EntityName = {
  type t = string;
};
module PropertyName = {
  type t = string;
};
module Identifier = {
  type t = string;
};

module Modifier = {
  type const = [ | `Const];
  type declare = [ | `Declare];
  type default = [ | `Default];
  type export = [ | `Export];
  type private = [ | `Private];
  type protected = [ | `Protected];
  type public = [ | `Public];
  type readonly = [ | `Readonly];
  type static = [ | `Static];
  type t = [
    const
    | declare
    | default
    | export
    | private
    | protected
    | public
    | readonly
    | static
  ];

  type accessibility = [ public | private | protected];
  type parameterProperty = [ accessibility | readonly];
  type classMember = [ accessibility | readonly | static];
};

type declaration =
  | TypeParameter(t(t_declaration_type_parameter))
  | TypeAlias(t(t_declaration_type_alias))
and t_declaration_type_alias = {
  name: Identifier.t,
  type_parameters: option(list(t(t_declaration_type_parameter))),
  type_: t(type_),
}
and t_declaration_type_parameter = {
  name: Identifier.t,
  constraint_: option(type_),
  default: option(type_),
}
and type_ =
  | TypePredicate(t(unit))
  | TypeReference(t(unit))
  | FunctionType(t(unit))
  | ConstructorType(t(unit))
  | TypeQuery(t(unit))
  | TypeLiteral(t(unit))
  | ArrayType(t(unit))
  | TupleType(t(unit))
  | NamedTupleMember(t(unit))
  | OptionalType(t(unit))
  | RestType(t(unit))
  | UnionType(t(unit))
  | IntersectionType(t(unit))
  | ConditionalType(t(unit))
  | InferType(t(unit))
  | ParenthesizedType(t(unit))
  | ThisType(t(unit))
  | TypeOperator(t(unit))
  | IndexedAccessType(t(unit))
  | MappedType(t(unit))
  | LiteralType(t(unit))
  | ImportType(t(t_node_import_type))
  | ExpressionWithTypeArguments(t(unit))
  | Keyword(t(keyword_kind))
and t_node_import_type = {
  is_typeof: bool,
  argument: type_,
  qualifier: option(EntityName.t),
}
and keyword_kind =
  | Any
  | BigInt
  | Boolean
  | Never
  | Number
  | Object
  | String
  | Symbol
  | Undefined
  | Unknown
  | Void
and t_function_or_constructor = {
  type_: t(type_),
  name: option(PropertyName.t),
  type_parameters: list(unit),
}
and t('a) = {
  pos: (Parse_info.t, option(Parse_info.t)),
  flags: Flags.Node.t,
  modifier_flags: Flags.Modifier.t,
  modifiers: option(array(Modifier.t)),
  id: option(int),
  symbol: option(unit),
  data: 'a,
};
