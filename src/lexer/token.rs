#[derive(Debug, Clone, PartialEq)]
pub enum Token {
  Identifier {
    line: usize,
    column: usize,

    namespace: Vec<String>,
    name: String,
  },

  LiteralString {
    line: usize,
    column: usize,
    value: String,
  },

  KeywordFunc {
    line: usize,
    column: usize,
  },

  ParenOpen {
    line: usize,
    column: usize,
  },
  ParenClose {
    line: usize,
    column: usize,
  },
  Colon {
    line: usize,
    column: usize,
  },

  Indent {
    line: usize,
    column: usize,
  },
  Dedent {
    line: usize,
    column: usize,
  },
}
