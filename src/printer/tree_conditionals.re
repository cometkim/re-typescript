open Tree_types;
type conditional_result;

module Flags = {
  let anyOrUnkown =
    fun
    | Base(Any | Unknown) => true
    | _ => false;
  let nullable =
    fun
    | Nullable(_)
    | Optional(_) => true
    | _ => false;
  let literal =
    fun
    | Literal(_) => true
    | _ => false;
  let definitelyFalsy =
    fun
    | Literal(_)
    | Base(Void | Undefined)
    | Nullable(_) => true
    | _ => false;
  let possiblyFalsy = v =>
    definitelyFalsy(v)
    || (
      switch (v) {
      | Base(String | Number | Boolean) => true
      | _ => false
      }
    );
  let stringLike =
    fun
    | Literal(`String(_))
    | EnumLiteral(`String(_))
    | Base(String) => true
    | _ => false;
  let never =
    fun
    | Base(Never) => true
    | _ => false;
  let string =
    fun
    | Base(String) => true
    | _ => false;
  let stringLiteral =
    fun
    | Literal(`String(_)) => true
    | _ => false;
  let enumLiteral =
    fun
    | EnumLiteral(_) => true
    | _ => false;
  let numberLike =
    fun
    | Base(Number)
    | Literal(`Number(_))
    | EnumLiteral(`Number(_)) => true
    | _ => false;
  let number =
    fun
    | Base(Number) => true
    | _ => false;
  let booleanLike =
    fun
    | Base(Boolean)
    | Literal(`TrueLiteral | `FalseLiteral) => true
    | _ => false;
  let boolean =
    fun
    | Base(Boolean) => true
    | _ => false;
};

let isSimpleTypeRelatedTo = (~source: ts_type, ~target: ts_type) => {
  open Flags;

  let s = source;
  let t = target;

  if (anyOrUnkown(t) || never(s)) {
    true;
  } else if (never(t)) {
    true;
  } else if (stringLike(s) && string(t)) {
    true;
  } else if (switch (s, t) {
             | (EnumLiteral(`String(s_value)), Literal(`String(t_value)))
                 when s_value === t_value =>
               true
             | _ => false
             }) {
    true;
  } else if (numberLike(s) && number(t)) {
    true;
  } else if (switch (s, t) {
             | (EnumLiteral(`Number(s_value)), Literal(`Number(t_value)))
                 when s_value === t_value =>
               true
             | _ => false
             }) {
    true;
  } else if (booleanLike(s) && boolean(t)) {
    true;
  } else {
    false;
  };
};
