use std::fmt::Display;

#[derive(Debug, Clone, PartialEq)]
pub struct TokenLocation {
  file_name: String,
  line: usize,
  column: usize,
  near: Option<char>,
}

impl TokenLocation {
  pub fn new(
    file_name: &str,
    line: usize,
    column: usize,
    near: Option<char>,
  ) -> Self {
    Self {
      file_name: file_name.into(),
      line,
      column,
      near,
    }
  }
}

impl Display for TokenLocation {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    match self.near {
      None => write!(
        f,
        "{}:{}:{}, near the end of file.",
        self.file_name, self.line, self.column
      ),
      Some(c) => write!(
        f,
        "{}:{}:{}, near the {:?}.",
        self.file_name, self.line, self.column, c
      ),
    }
  }
}
