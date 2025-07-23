#include "../lang.h"

namespace vc
{
  const auto wfParserTokens =
    // Structure.
    Paren | Bracket | Brace | List |
    // Literals.
    wfLiteral | String | RawString |
    // Symbols.
    Ident | SymbolId | DontCare | Equals | Const | TripleColon | DoubleColon |
    Colon | Vararg | Dot |
    // Keywords.
    Use | Where | Let | Var | Ref | Try | If | Else | While | For | Break |
    Continue | When | Return | Raise | Throw | New;

  // clang-format off
  const auto wfParser =
      (Top <<= (Directory | File)++)
    | (Directory <<= (Directory | File)++)
    | (File <<= Group++)
    | (Paren <<= (Group | List)++)
    | (Bracket <<= (Group | List)++)
    | (Brace <<= (Group | List)++)
    | (List <<= Group++)
    | (Group <<= wfParserTokens++)
    ;
  // clang-format on

  const std::initializer_list<Token> terminators = {List};

  Parse parser(std::shared_ptr<Bytecode> state)
  {
    struct ParseState
    {
      RE2 re_dir;
      size_t depth;
      size_t str_start;
      size_t str_end;
      Location loc;

      ParseState() : re_dir("[_[:alpha:]][_[:alnum:]]*?") {}
    };

    Parse p(depth::subdirectories, wfParser);
    auto ps = std::make_shared<ParseState>();

    p.prefile([](auto&, auto& path) { return path.extension() == ".v"; });

    p.predir([=](auto&, auto& path) {
      return RE2::FullMatch(path.filename().string(), ps->re_dir);
    });

    p.postfile([=](auto&, auto&, auto) { ps->depth = 0; });

    p.postparse([=](auto& pp, auto& path, auto ast) {
      state->set_path(path);
      auto stdlib = pp.executable().parent_path() / "std";

      if (path != stdlib)
        ast << pp.sub_parse(stdlib);

      return 0;
    });

    p("start",
      {
        // Whitespace between tokens.
        "[[:space:]]+" >> [](auto&) {},

        // Expression terminator.
        ";" >> [](auto& m) { m.term(terminators); },

        // Grouping.
        "\\(" >> [](auto& m) { m.push(Paren); },
        "\\)" >>
          [](auto& m) {
            m.term(terminators);
            m.pop(Paren);
          },
        "\\[" >> [](auto& m) { m.push(Bracket); },
        "\\]" >>
          [](auto& m) {
            m.term(terminators);
            m.pop(Bracket);
          },
        "\\{" >> [](auto& m) { m.push(Brace); },
        "\\}[[:space:]]*else\\b" >>
          [](auto& m) {
            // TODO: what if there's a comment in between } and `else`?
            // Braces don't terminate a group if followed by `else`.
            m.term(terminators);
            m.pop(Brace);
            m.add(Else);
          },
        "\\}" >>
          [](auto& m) {
            // Braces terminate a group, but not a list.
            m.term(terminators);
            m.pop(Brace);
            m.term();
          },

        // Bool.
        "true\\b" >> [](auto& m) { m.add(True); },
        "false\\b" >> [](auto& m) { m.add(False); },

        // Hex float.
        "[-]?0x[_[:xdigit:]]+\\.[_[:xdigit:]]+(?:p[+-][_[:digit:]]+)?\\b" >>
          [](auto& m) { m.add(HexFloat); },

        // Float.
        "[-]?[[:digit:]]+\\.[[:digit:]]+(?:e[+-]?[[:digit:]]+)?\\b" >>
          [](auto& m) { m.add(Float); },

        // Bin.
        "0b[01]+\\b" >> [](auto& m) { m.add(Bin); },

        // Oct.
        "0o[01234567]+\\b" >> [](auto& m) { m.add(Oct); },

        // Hex.
        "0x[[:xdigit:]]+\\b" >> [](auto& m) { m.add(Hex); },

        // Int.
        "[-]?[[:digit:]]+\\b" >> [](auto& m) { m.add(Int); },

        // Escaped string.
        "\"((?:\\\"|[^\"])*?)\"" >> [](auto& m) { m.add(String, 1); },

        // Raw string.
        "([']+)\"([^\"]*)" >>
          [=](auto& m) {
            ps->str_start = m.match(1).len;
            ps->str_end = 0;
            ps->loc = m.match(1);
            m.add(RawString, 2);
            m.mode("string");
          },

        // TODO: Character literal.
        // "'((?:\\'|[^'])*)'" >> [](auto& m) { m.add(Char, 1); },

        // Line comment.
        "//[^\r\n]*" >> [](auto&) {},

        // Nested comment.
        "/\\*" >>
          [=](auto& m) {
            assert(ps->depth == 0);
            ++ps->depth;
            ps->loc = m.match();
            m.mode("comment");
          },

        // Symbol that starts with `=` or `#`.
        "[=#][!#$%&*+-/<=>?@\\^`|~]+" >> [](auto& m) { m.add(SymbolId); },

        // Symbols.
        "=" >> [](auto& m) { m.add(Equals); },
        "#" >> [](auto& m) { m.add(Const); },
        "," >> [](auto& m) { m.seq(List); },
        ":::" >> [](auto& m) { m.add(TripleColon); },
        "::" >> [](auto& m) { m.add(DoubleColon); },
        ":" >> [](auto& m) { m.add(Colon); },
        "\\.\\.\\." >> [](auto& m) { m.add(Vararg); },
        "\\." >> [](auto& m) { m.add(Dot); },

        // Other symbols. Reserved: "'(),.:;[]_{}
        "[!#$%&*+-/<=>?@\\^`|~]+" >> [](auto& m) { m.add(SymbolId); },

        // Don't care.
        "_\\b" >> [](auto& m) { m.add(DontCare); },

        // Keywords.
        "use\\b" >> [](auto& m) { m.add(Use); },
        "where\\b" >> [](auto& m) { m.add(Where); },
        "let\\b" >> [](auto& m) { m.add(Let); },
        "var\\b" >> [](auto& m) { m.add(Var); },
        "ref\\b" >> [](auto& m) { m.add(Ref); },
        "try\\b" >> [](auto& m) { m.add(Try); },
        "if\\b" >> [](auto& m) { m.add(If); },
        "else\\b" >> [](auto& m) { m.add(Else); },
        "while\\b" >> [](auto& m) { m.add(While); },
        "for\\b" >> [](auto& m) { m.add(For); },
        "when\\b" >> [](auto& m) { m.add(When); },
        "break\\b" >> [](auto& m) { m.add(Break); },
        "continue\\b" >> [](auto& m) { m.add(Continue); },
        "return\\b" >> [](auto& m) { m.add(Return); },
        "raise\\b" >> [](auto& m) { m.add(Raise); },
        "throw\\b" >> [](auto& m) { m.add(Throw); },
        "new\\b" >> [](auto& m) { m.add(New); },

        // Identifier.
        "[_[:alpha:]][_[:alnum:]]*\\b" >> [](auto& m) { m.add(Ident); },
      });

    p("string",
      {
        "\"'" >>
          [=](auto& m) {
            m.extend_before(RawString);
            ps->str_end = 1;
            if (ps->str_start == ps->str_end)
              m.mode("start");
          },

        "'" >>
          [=](auto& m) {
            if (ps->str_end > 0)
            {
              ++ps->str_end;
              if (ps->str_start == ps->str_end)
                m.mode("start");
            }
          },

        "." >> [=](auto&) { ps->str_end = 0; },
      });

    p("comment",
      {
        "[^/\\*]+" >> [](auto&) {},
        "/\\*" >> [=](auto&) { ++ps->depth; },

        "\\*/" >>
          [=](auto& m) {
            if (--ps->depth == 0)
              m.mode("start");
          },

        "[/\\*]" >> [](auto&) {},
      });

    p.done([=](auto& m) {
      if (m.mode() == "comment")
        m.error("Unterminated comment starting at ", ps->loc);

      if (m.mode() == "string")
        m.error("Unterminated string starting at ", ps->loc);
    });

    return p;
  }
}
