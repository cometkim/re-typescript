open Re_typescript_base;

let content = {|
// This is a comment
export function someFunction(): void;
|};

let () = {
  let lexbuf = Lexing.from_string(content |> CCString.trim);

  try(
    {
      print_newline();
      Console.log("--------------------------------------");
      Console.log(
        Re_typescript_printer.print_from_ts(
          ~ctx={
            ...Re_typescript_printer.Config.default_config,
            number_mode: Int,
            output_type:
              Bucklescript({
                ...Re_typescript_printer.Config.default_bucklescript_config,
                string_variant_mode: `BsInline,
              }),
          },
          Parser_incr.parse(lexbuf),
        ),
      );
      Console.log("--------------------------------------");
      print_newline();
    }
  ) {
  | Lexer.SyntaxError(msg) => Printf.fprintf(stderr, "%s", msg)
  | Parser_incr.Parsing_error(_)
  | Parser.Error => Console.error(Error.parser_error(~content, ~lexbuf))
  | e =>
    Console.error(e);
    raise(e);
  };
};
