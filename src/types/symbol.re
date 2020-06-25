module rec SymbolTable: Core.Map.TMap =
  Core.Map.Make({
    type value = Symbol.t;
  })
and Symbol: {
  type t = {
    flags: Flags.Symbol.t,
    escaped_name: string,
    declarations: list(Declaration.t),
    value_declaration: Declaration.t,
    members: option(SymbolTable.t),
    exports: option(SymbolTable.t),
    globalExports: option(SymbolTable.t),
    id: option(int),
    mergeId: option(int),
    parent: option(t),
    exportSymbol: option(t),
    isReferenced: option(Flags.Symbol.t),
    isAssigned: option(bool),
    assignmentDeclarationMembers: option(Declaration.DeclarationMap.t),
  };
} = Symbol;

include Symbol;
