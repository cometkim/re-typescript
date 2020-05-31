# re-typescript

An opinionated attempt at finally solving typescript interop for `ReasonML` / `OCaml`.

## Why

I am maintaining a package of automatically generated bindings for `Material UI`. Generating these bindings is becoming more and more frustrating, as the complexity of the original package grows and tools that wish to cover everything begin to break down. this leads to more and more types missing on each cycle and precious time spent on recovering them by adjusting the type extraction process. Right now the generator uses a library which converts `typescript` into a `json-schema` which then get's analyzed & parsed into reason code. I believe that these tools want to cover too many use cases in order to be effective. So I'm setting out to simplify this process and maybe provide an effective way of generalizing type mapping between `reason` and `typescript`.

Quite ambitious. We'll see.

## Idea

`re-typescript` implements it's own lexer / parser to analyse typescript declaration files specifically. It purposefully does **not** want to get into tracking inferred types through `*.ts` files. It only extracts cleanly defined types as best as it can.

I am not sure about how to handle unparseable code yet. I believe a _best approximation_ appraoch will be best, where it will gracefully end the traversal by just injecting an abstract type of some sort, instead of failing or fully omitting a type.

`re-typescript` first derives a primitive AST from the tokenization process. It then decodes that AST by way of resolving references / dependencies. Finally it directly generates an ocaml AST, which allows for easy printing of both `reason` & `ocaml`. I'm composing the ast by using `ocaml-migrate-parsetree`, which will later allow for an easy use inside of a potential `PPX`.

This tool will be opinionated about a few things. Typescripts dynamic nature makes this unavoidable. I am not yet sure how the native mode will generate it's types exactly (esp. regarding unions / mixed enums). Bucklescript has great features for handling these things, like the `@unboxed` attribute, as well as `@bs.string` for externals and `bs.as` in records.

## Playground

I will update the playground from time to time. It's not great yet, I just wanted a visual representation for now. You can see the current progress here:
[re-typescript](https://jsiebern.github.io/re-typescript/)

## Todo

- [ ] Lexing / Parsing

  - [x] Include lexing positions for better error messages
  - [x] Basic
    - [x] Any
    - [x] Void
    - [x] Boolean
    - [x] Number
    - [x] String
    - [x] Null
    - [x] Undefined
    - [x] Symbol
    - [x] Reference
    - [x] Type extraction (`e.g. x['field']`)
  - [x] Array
  - [x] Tuple
  - [ ] Named tuple (derives from Array<T>)
  - [x] Union
    - [x] Simple Nullable / Optional
    - [x] String unions
    - [x] Int unions
    - [x] Mixed unions
    - [x] Union of types
  - [ ] Enum
    - [x] Keys
    - [x] Default values
    - [ ] Computed values
    - [x] Const enums
  - [x] Intersection types
  - [x] Interfaces / Objects
    - [x] Keyword
    - [x] Keys
    - [x] Semicolon / Comma
    - [x] Optional
    - [x] Readonly
    - [x] Nested interfaces
    - [x] Dynamic keys
    - [x] Extends keyword
  - [x] Functions
    - [x] By keyword
    - [x] Arrow function
    - [x] Arguments
    - [x] Return type
    - [ ] Fix arrow function problems
    - [x] Method signatures
  - [x] Classes
    - [x] Properties
    - [x] Static
    - [x] Public / Protected / Private
    - [x] Methods
    - [x] Inheritance
    - [x] Implementations
    - [x] Short fields initialization
  - [x] Imports
    - [x] Named
    - [x] Aliased
    - [x] Star aliased
    - [x] List of named
  - [x] Namespaces
  - [x] Exports
  - [x] Value bindings
  - [ ] Mutators
  - [ ] Conditional types
  - [x] Keyof (in keyof) types
  - [x] Type Parameters
    - [x] Type definition
    - [x] Interfaces
    - [x] Classes
    - [x] Extends keyword

- [ ] Decoding
  - [ ] Create representation
    - [x] Basic types
    - [x] Interfaces
      - [x] Nested
      - [ ] Empty interfaces that extend another can be set to the ref instead of inheriting the fields
      - [ ] Inline interfaces that use an arg need to carry the args in their definitions
    - [x] Arrays / Lists
    - [ ] Enums
    - [x] Unions
    - [ ] Functions
    - [ ] Classes
    - [ ] Intersection types
    - [x] Type Parameters
      - [ ] Throw error if 2 type parameters have the same name
  - [x] Abstracted file loader to resolve import / exports (should work both on web / native)
  - [ ] Implement separate file loaders
  - [x] Resolve extension refs for the current type (finalize all fields)
  - [x] Flattened type tree
  - [x] Solve inline interfaces (e.g. in an array)
  - [ ] Mutators
- [ ] Generating
  - [x] Valid identifiers (Reserved keywords / uppercase variables)
  - [ ] Native (Generate types without bucklescript features)
    - [ ] Basic types
    - [ ] Interfaces
    - [ ] Arrays / Lists
    - [ ] Enums
    - [ ] Unions
    - [ ] Functions
    - [ ] Classes
    - [ ] Intersection types
    - [ ] Type Parameters
  - [ ] Bucklescript (Make use of bucklescript features, but do not use externals)
    - [x] Basic types
    - [x] Interfaces
      - [x] `@bs.as` attribute
    - [x] Arrays / Lists
    - [x] Enums
    - [x] Unions
    - [x] Functions
      - [ ] String / Numeric literals in arguments
    - [ ] Classes
    - [ ] Namespaces
    - [ ] Intersection types
    - [x] Type Parameters
  - [ ] Bucklescript Bindings (Full use of bucklescript features and sensible approach of providing externals)
    - [ ] Basic types
    - [ ] Interfaces
    - [ ] Arrays / Lists
    - [ ] Enums
    - [ ] Unions
    - [ ] Functions
    - [ ] Classes
    - [ ] Exports as externals
    - [ ] Intersection types
    - [ ] Type Parameters
- [ ] Plugin system
  - [ ] ReasonReact
  - [ ] AST Structure
- [ ] Add more parsers / backends
  - [ ] Flow
- [ ] Playground
  - [ ] Performance improvements
  - [x] Check if it's really necessary to pull half of the ocaml compiler in (solved with `jsoo`)
  - [ ] Create a UI
    - [ ] Add panel with different generator options
  - [x] Add syntax highlighting
  - [ ] Keep source in local storage
  - [x] Add reason / ocaml switch
  - [ ] Download file button
- [ ] Config / Misc
  - [ ] Think of a way to inject overrides (e.g. a Hashmap of type name regexes and a replacement)
  - [x] Provide better error messages on syntax error
  - [x] `array_mode` (Array / List)
  - [x] `number_mode` (Int / Float / Unboxed)
  - [ ] `uncurried_functions_mode`
  - [ ] `omit_labels_for_required_args`
  - [ ] Hoist inline variants when generating parsers / serializers (for ease of use together with [@bs.deriving])
  - [ ] Option to throw error if a type can't be resolved while decoding
  - [ ] PPX (far future)
    - ```reason
      [%re_typescript "package/def.d.ts"]]
      ```
    - ```reason
      [%re_typescript "package/def.d.ts"(["a_type", "another_type"])]]
      ```
    - ```reason
      [%re_typescript {| interface { field: string; } |}]]
      ```
