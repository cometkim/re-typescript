(* Auto-generated from "typescript.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type symbol = Typescript_t.symbol = {
  flags: int;
  id: int option;
  fullyQualifiedName: string;
  name: string
}

type node = Typescript_t.node

and node_ArrayType = Typescript_t.node_ArrayType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  elementType: node
}

and node_BindingElement = Typescript_t.node_BindingElement = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  propertyName: node option;
  dotDotDotToken: node option;
  name: node;
  initializer_: node option
}

and node_CallSignature = Typescript_t.node_CallSignature = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option
}

and node_EnumDeclaration = Typescript_t.node_EnumDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node;
  members: node list
}

and node_EnumMember = Typescript_t.node_EnumMember = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node;
  initializer_: node option
}

and node_FunctionDeclaration = Typescript_t.node_FunctionDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option;
  asteriskToken: node option;
  questionToken: node option;
  exclamationToken: node option;
  name: node option;
  body: node option
}

and node_FunctionType = Typescript_t.node_FunctionType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option
}

and node_Generic = Typescript_t.node_Generic = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option
}

and node_HeritageClause = Typescript_t.node_HeritageClause = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  token: int;
  types: node list option
}

and node_Identifier = Typescript_t.node_Identifier = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  escapedText: string;
  originalKeywordKind: int option
}

and node_IndexSignature = Typescript_t.node_IndexSignature = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option;
  name: node option
}

and node_IndexedAccessType = Typescript_t.node_IndexedAccessType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  objectType: node;
  indexType: node
}

and node_InterfaceDeclaration = Typescript_t.node_InterfaceDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node;
  typeParameters: node list option;
  members: node list
}

and node_IntersectionType = Typescript_t.node_IntersectionType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  types: node list
}

and node_LiteralLikeNode = Typescript_t.node_LiteralLikeNode = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  text: string;
  isUnterminated: bool option;
  hasExtendedUnicodeEscape: bool option
}

and node_LiteralType = Typescript_t.node_LiteralType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  literal: node
}

and node_MethodSignature = Typescript_t.node_MethodSignature = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option;
  name: node option
}

and node_OptionalType = Typescript_t.node_OptionalType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  type_: node
}

and node_Parameter = Typescript_t.node_Parameter = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  dotDotDotToken: node option;
  name: node;
  questionToken: node option;
  type_: node option;
  initializer_: node option
}

and node_ParenthesizedType = Typescript_t.node_ParenthesizedType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  type_: node
}

and node_PropertyDeclaration = Typescript_t.node_PropertyDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node;
  questionToken: node option;
  exclamationToken: node option;
  type_: node option;
  initializer_: node option
}

and node_PropertySignature = Typescript_t.node_PropertySignature = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node;
  questionToken: node option;
  type_: node option;
  initializer_: node option
}

and node_QualifiedName = Typescript_t.node_QualifiedName = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  left: node;
  right: node
}

and node_RestType = Typescript_t.node_RestType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  type_: node
}

and node_SourceFile = Typescript_t.node_SourceFile = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  statements: node list;
  text: string;
  languageVersion: int;
  fileName: string;
  languageVariant: int;
  isDeclarationFile: bool;
  nodeCount: int;
  identifierCount: int
}

and node_TupleType = Typescript_t.node_TupleType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  elementTypes: node list
}

and node_TypeAliasDeclaration = Typescript_t.node_TypeAliasDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node;
  typeParameters: node list option;
  type_: node
}

and node_TypeLiteral = Typescript_t.node_TypeLiteral = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  members: node list
}

and node_TypeOperator = Typescript_t.node_TypeOperator = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  operator: int;
  type_: node
}

and node_TypeParameter = Typescript_t.node_TypeParameter = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node;
  constraint_: node option;
  default: node option;
  expression: node option
}

and node_TypeReference = Typescript_t.node_TypeReference = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  typeArguments: node list option;
  typeName: node
}

and node_UnionType = Typescript_t.node_UnionType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  types: node list
}

and node_VariableDeclaration = Typescript_t.node_VariableDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node;
  exclamationToken: node option;
  type_: node option;
  initializer_: node option
}

and node_VariableDeclarationList =
  Typescript_t.node_VariableDeclarationList = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  declarations: node list
}

and type_ = Typescript_t.type_

and type_AnonymousType = Typescript_t.type_AnonymousType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  objectFlags: int;
  target: type_ option
}

and type_Generic = Typescript_t.type_Generic = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string
}

and type_InterfaceType = Typescript_t.type_InterfaceType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  objectFlags: int;
  typeParameters: type_ list option;
  outerTypeParameters: type_ list option;
  localTypeParameters: type_ list option;
  thisType: type_ option
}

and type_IntersectionType = Typescript_t.type_IntersectionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  types: type_ list
}

and type_NumberLiteralType = Typescript_t.type_NumberLiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  value: float
}

and type_ObjectType = Typescript_t.type_ObjectType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  objectFlags: int
}

and type_StringLiteralType = Typescript_t.type_StringLiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  value: string
}

and type_TupleType = Typescript_t.type_TupleType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  minLength: int;
  fixedLength: int;
  hasRestElement: bool;
  readonly: bool;
  elementFlags: int list;
  combinedFlags: int;
  labeledElementDeclarations: node list option
}

and type_TypeParameter = Typescript_t.type_TypeParameter = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string
}

and type_TypeReference = Typescript_t.type_TypeReference = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  target: type_ option
}

and type_Unidentified = Typescript_t.type_Unidentified = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  nodeKind: string option
}

and type_UnionOrIntersectionType =
  Typescript_t.type_UnionOrIntersectionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  types: type_ list
}

and type_UnionType = Typescript_t.type_UnionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  types: type_ list
}

type conditionalRoot = Typescript_t.conditionalRoot = {
  node: node;
  checkType: type_;
  extendsType: type_;
  trueType: type_;
  falseType: type_;
  isDistributive: bool;
  inferTypeParameters: type_ list option;
  outerTypeParameters: type_ list option;
  instantiations: (string * type_) option;
  aliasSymbol: symbol option;
  aliasTypeArguments: type_ list option
}

type type_conditionalType = Typescript_t.type_conditionalType = {
  root: conditionalRoot;
  checkType: type_;
  extendsType: type_;
  resolvedTrueType: type_;
  resolvedFalseType: type_
}

type type_LiteralType = Typescript_t.type_LiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string
}

type readonlyTextRange = Typescript_t.readonlyTextRange = {
  pos: int;
  end_: int
}

type node_TypeElement = Typescript_t.node_TypeElement = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  questionToken: node option
}

type node_SignatureDeclarationBase =
  Typescript_t.node_SignatureDeclarationBase = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option
}

type node_NodeWithTypeArguments = Typescript_t.node_NodeWithTypeArguments = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  typeArguments: node list option
}

type node_NamedTupleMember = Typescript_t.node_NamedTupleMember = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  dotDotDotToken: node option;
  questionToken: node option;
  name: node;
  type_: node
}

type node_NamedDeclaration = Typescript_t.node_NamedDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node option
}

type node_FunctionLikeDeclarationBase =
  Typescript_t.node_FunctionLikeDeclarationBase = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeNode: node option;
  name: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option;
  asteriskToken: node option;
  questionToken: node option;
  exclamationToken: node option;
  body: node option
}

val read_symbol :  symbol Atdgen_codec_runtime.Decode.t

val write_symbol :  symbol Atdgen_codec_runtime.Encode.t

val read_node :  node Atdgen_codec_runtime.Decode.t

val write_node :  node Atdgen_codec_runtime.Encode.t

val read_node_ArrayType :  node_ArrayType Atdgen_codec_runtime.Decode.t

val write_node_ArrayType :  node_ArrayType Atdgen_codec_runtime.Encode.t

val read_node_BindingElement :  node_BindingElement Atdgen_codec_runtime.Decode.t

val write_node_BindingElement :  node_BindingElement Atdgen_codec_runtime.Encode.t

val read_node_CallSignature :  node_CallSignature Atdgen_codec_runtime.Decode.t

val write_node_CallSignature :  node_CallSignature Atdgen_codec_runtime.Encode.t

val read_node_EnumDeclaration :  node_EnumDeclaration Atdgen_codec_runtime.Decode.t

val write_node_EnumDeclaration :  node_EnumDeclaration Atdgen_codec_runtime.Encode.t

val read_node_EnumMember :  node_EnumMember Atdgen_codec_runtime.Decode.t

val write_node_EnumMember :  node_EnumMember Atdgen_codec_runtime.Encode.t

val read_node_FunctionDeclaration :  node_FunctionDeclaration Atdgen_codec_runtime.Decode.t

val write_node_FunctionDeclaration :  node_FunctionDeclaration Atdgen_codec_runtime.Encode.t

val read_node_FunctionType :  node_FunctionType Atdgen_codec_runtime.Decode.t

val write_node_FunctionType :  node_FunctionType Atdgen_codec_runtime.Encode.t

val read_node_Generic :  node_Generic Atdgen_codec_runtime.Decode.t

val write_node_Generic :  node_Generic Atdgen_codec_runtime.Encode.t

val read_node_HeritageClause :  node_HeritageClause Atdgen_codec_runtime.Decode.t

val write_node_HeritageClause :  node_HeritageClause Atdgen_codec_runtime.Encode.t

val read_node_Identifier :  node_Identifier Atdgen_codec_runtime.Decode.t

val write_node_Identifier :  node_Identifier Atdgen_codec_runtime.Encode.t

val read_node_IndexSignature :  node_IndexSignature Atdgen_codec_runtime.Decode.t

val write_node_IndexSignature :  node_IndexSignature Atdgen_codec_runtime.Encode.t

val read_node_IndexedAccessType :  node_IndexedAccessType Atdgen_codec_runtime.Decode.t

val write_node_IndexedAccessType :  node_IndexedAccessType Atdgen_codec_runtime.Encode.t

val read_node_InterfaceDeclaration :  node_InterfaceDeclaration Atdgen_codec_runtime.Decode.t

val write_node_InterfaceDeclaration :  node_InterfaceDeclaration Atdgen_codec_runtime.Encode.t

val read_node_IntersectionType :  node_IntersectionType Atdgen_codec_runtime.Decode.t

val write_node_IntersectionType :  node_IntersectionType Atdgen_codec_runtime.Encode.t

val read_node_LiteralLikeNode :  node_LiteralLikeNode Atdgen_codec_runtime.Decode.t

val write_node_LiteralLikeNode :  node_LiteralLikeNode Atdgen_codec_runtime.Encode.t

val read_node_LiteralType :  node_LiteralType Atdgen_codec_runtime.Decode.t

val write_node_LiteralType :  node_LiteralType Atdgen_codec_runtime.Encode.t

val read_node_MethodSignature :  node_MethodSignature Atdgen_codec_runtime.Decode.t

val write_node_MethodSignature :  node_MethodSignature Atdgen_codec_runtime.Encode.t

val read_node_OptionalType :  node_OptionalType Atdgen_codec_runtime.Decode.t

val write_node_OptionalType :  node_OptionalType Atdgen_codec_runtime.Encode.t

val read_node_Parameter :  node_Parameter Atdgen_codec_runtime.Decode.t

val write_node_Parameter :  node_Parameter Atdgen_codec_runtime.Encode.t

val read_node_ParenthesizedType :  node_ParenthesizedType Atdgen_codec_runtime.Decode.t

val write_node_ParenthesizedType :  node_ParenthesizedType Atdgen_codec_runtime.Encode.t

val read_node_PropertyDeclaration :  node_PropertyDeclaration Atdgen_codec_runtime.Decode.t

val write_node_PropertyDeclaration :  node_PropertyDeclaration Atdgen_codec_runtime.Encode.t

val read_node_PropertySignature :  node_PropertySignature Atdgen_codec_runtime.Decode.t

val write_node_PropertySignature :  node_PropertySignature Atdgen_codec_runtime.Encode.t

val read_node_QualifiedName :  node_QualifiedName Atdgen_codec_runtime.Decode.t

val write_node_QualifiedName :  node_QualifiedName Atdgen_codec_runtime.Encode.t

val read_node_RestType :  node_RestType Atdgen_codec_runtime.Decode.t

val write_node_RestType :  node_RestType Atdgen_codec_runtime.Encode.t

val read_node_SourceFile :  node_SourceFile Atdgen_codec_runtime.Decode.t

val write_node_SourceFile :  node_SourceFile Atdgen_codec_runtime.Encode.t

val read_node_TupleType :  node_TupleType Atdgen_codec_runtime.Decode.t

val write_node_TupleType :  node_TupleType Atdgen_codec_runtime.Encode.t

val read_node_TypeAliasDeclaration :  node_TypeAliasDeclaration Atdgen_codec_runtime.Decode.t

val write_node_TypeAliasDeclaration :  node_TypeAliasDeclaration Atdgen_codec_runtime.Encode.t

val read_node_TypeLiteral :  node_TypeLiteral Atdgen_codec_runtime.Decode.t

val write_node_TypeLiteral :  node_TypeLiteral Atdgen_codec_runtime.Encode.t

val read_node_TypeOperator :  node_TypeOperator Atdgen_codec_runtime.Decode.t

val write_node_TypeOperator :  node_TypeOperator Atdgen_codec_runtime.Encode.t

val read_node_TypeParameter :  node_TypeParameter Atdgen_codec_runtime.Decode.t

val write_node_TypeParameter :  node_TypeParameter Atdgen_codec_runtime.Encode.t

val read_node_TypeReference :  node_TypeReference Atdgen_codec_runtime.Decode.t

val write_node_TypeReference :  node_TypeReference Atdgen_codec_runtime.Encode.t

val read_node_UnionType :  node_UnionType Atdgen_codec_runtime.Decode.t

val write_node_UnionType :  node_UnionType Atdgen_codec_runtime.Encode.t

val read_node_VariableDeclaration :  node_VariableDeclaration Atdgen_codec_runtime.Decode.t

val write_node_VariableDeclaration :  node_VariableDeclaration Atdgen_codec_runtime.Encode.t

val read_node_VariableDeclarationList :  node_VariableDeclarationList Atdgen_codec_runtime.Decode.t

val write_node_VariableDeclarationList :  node_VariableDeclarationList Atdgen_codec_runtime.Encode.t

val read_type_ :  type_ Atdgen_codec_runtime.Decode.t

val write_type_ :  type_ Atdgen_codec_runtime.Encode.t

val read_type_AnonymousType :  type_AnonymousType Atdgen_codec_runtime.Decode.t

val write_type_AnonymousType :  type_AnonymousType Atdgen_codec_runtime.Encode.t

val read_type_Generic :  type_Generic Atdgen_codec_runtime.Decode.t

val write_type_Generic :  type_Generic Atdgen_codec_runtime.Encode.t

val read_type_InterfaceType :  type_InterfaceType Atdgen_codec_runtime.Decode.t

val write_type_InterfaceType :  type_InterfaceType Atdgen_codec_runtime.Encode.t

val read_type_IntersectionType :  type_IntersectionType Atdgen_codec_runtime.Decode.t

val write_type_IntersectionType :  type_IntersectionType Atdgen_codec_runtime.Encode.t

val read_type_NumberLiteralType :  type_NumberLiteralType Atdgen_codec_runtime.Decode.t

val write_type_NumberLiteralType :  type_NumberLiteralType Atdgen_codec_runtime.Encode.t

val read_type_ObjectType :  type_ObjectType Atdgen_codec_runtime.Decode.t

val write_type_ObjectType :  type_ObjectType Atdgen_codec_runtime.Encode.t

val read_type_StringLiteralType :  type_StringLiteralType Atdgen_codec_runtime.Decode.t

val write_type_StringLiteralType :  type_StringLiteralType Atdgen_codec_runtime.Encode.t

val read_type_TupleType :  type_TupleType Atdgen_codec_runtime.Decode.t

val write_type_TupleType :  type_TupleType Atdgen_codec_runtime.Encode.t

val read_type_TypeParameter :  type_TypeParameter Atdgen_codec_runtime.Decode.t

val write_type_TypeParameter :  type_TypeParameter Atdgen_codec_runtime.Encode.t

val read_type_TypeReference :  type_TypeReference Atdgen_codec_runtime.Decode.t

val write_type_TypeReference :  type_TypeReference Atdgen_codec_runtime.Encode.t

val read_type_Unidentified :  type_Unidentified Atdgen_codec_runtime.Decode.t

val write_type_Unidentified :  type_Unidentified Atdgen_codec_runtime.Encode.t

val read_type_UnionOrIntersectionType :  type_UnionOrIntersectionType Atdgen_codec_runtime.Decode.t

val write_type_UnionOrIntersectionType :  type_UnionOrIntersectionType Atdgen_codec_runtime.Encode.t

val read_type_UnionType :  type_UnionType Atdgen_codec_runtime.Decode.t

val write_type_UnionType :  type_UnionType Atdgen_codec_runtime.Encode.t

val read_conditionalRoot :  conditionalRoot Atdgen_codec_runtime.Decode.t

val write_conditionalRoot :  conditionalRoot Atdgen_codec_runtime.Encode.t

val read_type_conditionalType :  type_conditionalType Atdgen_codec_runtime.Decode.t

val write_type_conditionalType :  type_conditionalType Atdgen_codec_runtime.Encode.t

val read_type_LiteralType :  type_LiteralType Atdgen_codec_runtime.Decode.t

val write_type_LiteralType :  type_LiteralType Atdgen_codec_runtime.Encode.t

val read_readonlyTextRange :  readonlyTextRange Atdgen_codec_runtime.Decode.t

val write_readonlyTextRange :  readonlyTextRange Atdgen_codec_runtime.Encode.t

val read_node_TypeElement :  node_TypeElement Atdgen_codec_runtime.Decode.t

val write_node_TypeElement :  node_TypeElement Atdgen_codec_runtime.Encode.t

val read_node_SignatureDeclarationBase :  node_SignatureDeclarationBase Atdgen_codec_runtime.Decode.t

val write_node_SignatureDeclarationBase :  node_SignatureDeclarationBase Atdgen_codec_runtime.Encode.t

val read_node_NodeWithTypeArguments :  node_NodeWithTypeArguments Atdgen_codec_runtime.Decode.t

val write_node_NodeWithTypeArguments :  node_NodeWithTypeArguments Atdgen_codec_runtime.Encode.t

val read_node_NamedTupleMember :  node_NamedTupleMember Atdgen_codec_runtime.Decode.t

val write_node_NamedTupleMember :  node_NamedTupleMember Atdgen_codec_runtime.Encode.t

val read_node_NamedDeclaration :  node_NamedDeclaration Atdgen_codec_runtime.Decode.t

val write_node_NamedDeclaration :  node_NamedDeclaration Atdgen_codec_runtime.Encode.t

val read_node_FunctionLikeDeclarationBase :  node_FunctionLikeDeclarationBase Atdgen_codec_runtime.Decode.t

val write_node_FunctionLikeDeclarationBase :  node_FunctionLikeDeclarationBase Atdgen_codec_runtime.Encode.t

