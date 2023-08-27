pub fn char_is_whitespace_not_newline(c: char) -> bool {
  c.is_whitespace() && c != '\n'
}

pub fn char_is_not_double_quote(c: char) -> bool {
  c != '"'
}
