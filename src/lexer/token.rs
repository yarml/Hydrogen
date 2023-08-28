use super::location::TokenLocation;

#[derive(Debug, Clone, PartialEq)]
pub enum Token {
  Identifier {
    location: TokenLocation,

    namespace: Vec<String>,
    name: String,
  },

  LiteralString {
    location: TokenLocation,
    value: String,
  },

  KeywordFunc {
    location: TokenLocation,
  },
  KeywordDef {
    location: TokenLocation,
  },

  ParenOpen {
    location: TokenLocation,
  },
  ParenClose {
    location: TokenLocation,
  },
  Colon {
    location: TokenLocation,
  },
  Comma {
    location: TokenLocation,
  },

  Indent {
    location: TokenLocation,
  },
  Dedent {
    location: TokenLocation,
  },
}
