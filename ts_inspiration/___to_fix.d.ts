interface Dictionary<T> {
  [key: string]: T;
}
export let keys: keyof Dictionary<number>; // string | number
export let value: Dictionary<number>["foo"]; // number

//////////////////////////////////////////////////////////

type TypeName<T> = T extends string
  ? "string"
  : T extends number
  ? "number"
  : T extends boolean
  ? "boolean"
  : T extends undefined
  ? "undefined"
  : T extends Function
  ? "function"
  : "object";

type T0 = TypeName<string>; // "string"
type T1 = TypeName<"a">; // "string"
type T2 = TypeName<true>; // "boolean"
type T3 = TypeName<() => void>; // "function"
type T4 = TypeName<string[]>; // "object"

//////////////////////////////////////////////////////////

interface A {
  x: string;
  y: number;
  z: boolean;
}

type Exclude<T, U> = T extends U ? never : T;
type stripped = Exclude<keyof A, "y">;

export enum StringEnum {
  A = "A",
  B = "B",
}

export enum NumberEnum {
  A,
  B,
}

//////////////////////////////////////////////////////////

type StrictNumericEnumParam<
  Enum extends number,
  Param extends Enum
> = true extends ({ [key: number]: false } & { [P in Enum]: true })[Enum]
  ? true extends ({ [key: number]: false } & { [P in Enum]: true })[Param]
    ? Param
    : never
  : Enum;

export enum Foo {
  A,
  B,
  C,
}

export enum Bar {
  D,
  E,
  F,
}

// typically would be written as function doFoo(value: Foo): void
declare function doFoo<Value extends Foo>(
  value: StrictNumericEnumParam<Foo, Value>
): void;

declare const foo: Foo;
declare const bar: Bar;
declare const n: number;

type x = "xxx" extends string ? true : false;

/////////////////////////////
export type ConsistentWith<DecorationTargetProps, InjectedProps> = {
  [P in keyof DecorationTargetProps]: P extends keyof InjectedProps
    ? InjectedProps[P] extends DecorationTargetProps[P]
      ? DecorationTargetProps[P]
      : InjectedProps[P]
    : DecorationTargetProps[P];
};

type a = { key_1: number; key_2: number };
type b = { key_1: string; key_3: boolean };
type y = a extends ConsistentWith<a, b> ? true : false;

//// [deeplyNestedConditionalTypes.d.ts]
declare type Foox<T> = T extends 0
  ? "0"
  : T extends 1
  ? "1"
  : T extends 2
  ? "2"
  : T extends 3
  ? "3"
  : T extends 4
  ? "4"
  : T extends 5
  ? "5"
  : T extends 6
  ? "6"
  : T extends 7
  ? "7"
  : T extends 8
  ? "8"
  : T extends 9
  ? "9"
  : T extends 10
  ? "10"
  : T extends 11
  ? "11"
  : T extends 12
  ? "12"
  : T extends 13
  ? "13"
  : T extends 14
  ? "14"
  : T extends 15
  ? "15"
  : T extends 16
  ? "16"
  : T extends 17
  ? "17"
  : T extends 18
  ? "18"
  : T extends 19
  ? "19"
  : T extends 20
  ? "20"
  : T extends 21
  ? "21"
  : T extends 22
  ? "22"
  : T extends 23
  ? "23"
  : T extends 24
  ? "24"
  : T extends 25
  ? "25"
  : T extends 26
  ? "26"
  : T extends 27
  ? "27"
  : T extends 28
  ? "28"
  : T extends 29
  ? "29"
  : T extends 30
  ? "30"
  : T extends 31
  ? "31"
  : T extends 32
  ? "32"
  : T extends 33
  ? "33"
  : T extends 34
  ? "34"
  : T extends 35
  ? "35"
  : T extends 36
  ? "36"
  : T extends 37
  ? "37"
  : T extends 38
  ? "38"
  : T extends 39
  ? "39"
  : T extends 40
  ? "40"
  : T extends 41
  ? "41"
  : T extends 42
  ? "42"
  : T extends 43
  ? "43"
  : T extends 44
  ? "44"
  : T extends 45
  ? "45"
  : T extends 46
  ? "46"
  : T extends 47
  ? "47"
  : T extends 48
  ? "48"
  : T extends 49
  ? "49"
  : T extends 50
  ? "50"
  : T extends 51
  ? "51"
  : T extends 52
  ? "52"
  : T extends 53
  ? "53"
  : T extends 54
  ? "54"
  : T extends 55
  ? "55"
  : T extends 56
  ? "56"
  : T extends 57
  ? "57"
  : T extends 58
  ? "58"
  : T extends 59
  ? "59"
  : T extends 60
  ? "60"
  : T extends 61
  ? "61"
  : T extends 62
  ? "62"
  : T extends 63
  ? "63"
  : T extends 64
  ? "64"
  : T extends 65
  ? "65"
  : T extends 66
  ? "66"
  : T extends 67
  ? "67"
  : T extends 68
  ? "68"
  : T extends 69
  ? "69"
  : T extends 70
  ? "70"
  : T extends 71
  ? "71"
  : T extends 72
  ? "72"
  : T extends 73
  ? "73"
  : T extends 74
  ? "74"
  : T extends 75
  ? "75"
  : T extends 76
  ? "76"
  : T extends 77
  ? "77"
  : T extends 78
  ? "78"
  : T extends 79
  ? "79"
  : T extends 80
  ? "80"
  : T extends 81
  ? "81"
  : T extends 82
  ? "82"
  : T extends 83
  ? "83"
  : T extends 84
  ? "84"
  : T extends 85
  ? "85"
  : T extends 86
  ? "86"
  : T extends 87
  ? "87"
  : T extends 88
  ? "88"
  : T extends 89
  ? "89"
  : T extends 90
  ? "90"
  : T extends 91
  ? "91"
  : T extends 92
  ? "92"
  : T extends 93
  ? "93"
  : T extends 94
  ? "94"
  : T extends 95
  ? "95"
  : T extends 96
  ? "96"
  : T extends 97
  ? "97"
  : T extends 98
  ? "98"
  : T extends 99
  ? "99"
  : never;
declare type xx = Foox<99>;
declare type yy = Foox<any>;
