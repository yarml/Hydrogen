use super::location::TokenLocation;

#[derive(Debug, Clone, PartialEq)]
pub enum LexerError {
  InvalidState {
    location: TokenLocation,
  },
  UnexpectedSymbol {
    location: TokenLocation,
  },
  UnknownIndentationType {
    location: TokenLocation,
  },
  MixSpacesAndTabsIndentations {
    location: TokenLocation,
  },
  OddSpaceIndentation {
    location: TokenLocation,
  },
  MultipleIndentations {
    location: TokenLocation,
  },
  UnexpectedLiteralStringEnding {
    beg_location: TokenLocation,
    end_location: TokenLocation,
  },
}

impl LexerError {
  pub fn error_str(&self) -> String {
    match self {
      LexerError::InvalidState { location } => {
        format!("Invalid State at {location}")
      }
      LexerError::UnexpectedSymbol { location } => {
        format!("Unexpected symbol at {location}")
      }
      LexerError::UnknownIndentationType { location } => {
        format!("Unknown indentation type at {location}")
      }
      LexerError::MixSpacesAndTabsIndentations { location } => {
        format!("Indentation mixes spaces and tabs at {location}")
      }
      LexerError::OddSpaceIndentation { location } => format!(
        "Indentation contains a missing/additional space at {location}."
      ),
      LexerError::MultipleIndentations { location } => {
        format!("Lot of indentations at {location}")
      }
      LexerError::UnexpectedLiteralStringEnding {
        beg_location,
        end_location,
      } => {
        format!("File ended before string closed at {end_location}. Not string opened at {beg_location}")
      }
      #[allow(unreachable_patterns)]
      e => {
        format!("{:?}", e)
      }
    }
  }
}
