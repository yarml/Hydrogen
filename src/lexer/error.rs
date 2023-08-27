#[derive(Debug, Clone, PartialEq)]
pub enum LexerError {
  InvalidState {
    line: usize,
    column: usize,
    ch: Option<char>,
  },
  UnknownIndentationType {
    line: usize,
    column: usize,
  },
  MixSpacesAndTabsIndentations {
    line: usize,
    column: usize,
  },
  OddSpaceIndentation {
    line: usize,
    column: usize,
  },
  MultipleIndentations {
    line: usize,
    column: usize,
  },
  UnexpectedLiteralStringEnding {
    line: usize,
    column: usize,
  },
}

impl LexerError {
  pub fn error_str(&self) -> String {
    match *self {
      LexerError::InvalidState {
        line,
        column,
        ch: Some(c),
      } if column > 0 => format!(
        "Invalid State at {}:{}, near the character {:?}",
        line, column, c
      ),
      LexerError::InvalidState {
        line,
        column,
        ch: None,
      } if column > 0 => format!("Invalid State at {}:{}", line, column),
      LexerError::InvalidState {
        line,
        column,
        ch: Some(c),
      } if column <= 0 => {
        format!("Invalid State at line {}, near the character {:?}", line, c)
      }
      LexerError::InvalidState {
        line,
        column,
        ch: None,
      } if column <= 0 => {
        format!("Invalid State at line {}", line)
      }
      LexerError::InvalidState { .. } => format!("Invalid state"),
      LexerError::UnknownIndentationType { line, .. } => {
        format!("Unknown indentation type at line {}", line)
      }
      LexerError::MixSpacesAndTabsIndentations { line, .. } => {
        format!("Indentation mixes spaces and tabs at line {}", line)
      }
      LexerError::OddSpaceIndentation { line, .. } => format!(
        "Indentation contains a missing/additional space at line {}.",
        line
      ),
      LexerError::MultipleIndentations { line, .. } => {
        format!("Line {} has a lot of indentations", line)
      }
      LexerError::UnexpectedLiteralStringEnding { line, column } => {
        format!("File ended before string closed at {}:{}", line, column)
      }
    }
  }
}
