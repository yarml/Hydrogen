use queues::IsQueue;

use crate::lexer::error::LexerError;

use super::{
  token::Token,
  utils::{char_is_not_double_quote, char_is_whitespace_not_newline},
  Lexer, TokenResult,
};

impl<'source> Lexer<'source> {
  pub(super) fn next_real_token(&mut self) -> Option<TokenResult> {
    loop {
      match self.next_skip_char(char_is_whitespace_not_newline, None) {
        // End of file
        None => return None,
        // Comments
        Some('#') => match self.next_char() {
          None => return None,
          Some('\n') => self.putbackc('\n'),
          Some('#') => match self.next_until("##") {
            None => return None,
            Some(c) => self.putbackc(c),
          },
          Some(_) => match self.next_until("\n") {
            None => return None,
            Some(c) => {
              self.putbackc('\n');
              self.putbackc(c);
            }
          },
        },
        // One character symbols
        Some('(') => {
          return Some(Ok(Token::ParenOpen {
            line: self.line,
            column: self.column,
          }))
        }
        Some(')') => {
          return Some(Ok(Token::ParenClose {
            line: self.line,
            column: self.column,
          }))
        }
        Some(':') => {
          return Some(Ok(Token::Colon {
            line: self.line,
            column: self.column,
          }))
        }
        // Indents and dedents
        Some('\n') => {
          let mut spaces = String::new();
          match self
            .next_skip_char(char_is_whitespace_not_newline, Some(&mut spaces))
          {
            None => return None,
            Some('\n') => {
              self.putbackc('\n');
            }
            Some(c) => {
              self.putbackc(c);
              enum IndentType {
                Space,
                Tab,
              }
              impl IndentType {
                fn match_char(&self, c: char) -> bool {
                  match self {
                    Self::Space => c == ' ',
                    Self::Tab => c == '\t',
                  }
                }
              }
              let deduced_indent_type = match spaces.chars().nth(0) {
                Some(' ') => IndentType::Space,
                Some('\t') => IndentType::Tab,
                None => IndentType::Space,
                _ => {
                  return Some(Err(LexerError::UnknownIndentationType {
                    line: self.line,
                    column: 0,
                  }))
                }
              };
              let mut indentation = 0;
              for space in spaces.chars() {
                if !deduced_indent_type.match_char(space) {
                  return Some(Err(LexerError::MixSpacesAndTabsIndentations {
                    line: self.line,
                    column: 0,
                  }));
                }

                indentation += 1;
              }

              match deduced_indent_type {
                IndentType::Space if indentation % 2 != 0 => {
                  return Some(Err(LexerError::OddSpaceIndentation {
                    line: self.line,
                    column: 0,
                  }))
                }
                IndentType::Space => indentation /= 2,
                _ => {}
              }

              let old_indentation = self.indentation;
              self.indentation = indentation;

              match indentation as isize - old_indentation as isize {
                pdiff if pdiff > 1 => {
                  return Some(Err(LexerError::MultipleIndentations {
                    line: self.line,
                    column: 0,
                  }))
                }
                1 => {
                  return Some(Ok(Token::Indent {
                    line: self.line,
                    column: 0,
                  }))
                }
                0 => {}
                ndiff => {
                  for _ in 0..-((ndiff - 1) as isize) {
                    self
                      .next_buf
                      .add(Token::Dedent {
                        line: self.line,
                        column: 0,
                      })
                      .unwrap();
                  }
                  return Some(Ok(Token::Dedent {
                    line: self.line,
                    column: 0,
                  }));
                }
              }
            }
          }
        }
        // Identifiers & keywords
        Some(c) if c.is_alphabetic() => {
          let mut identifier = format!("{}", c);
          let next =
            self.next_skip_char(char::is_alphanumeric, Some(&mut identifier));
          if next.is_some() {
            self.putbackc(next.unwrap());
          }
          let token = match identifier.as_str() {
            "func" => Token::KeywordFunc {
              line: self.line,
              column: self.column,
            },
            _ => Token::Identifier {
              line: self.line,
              column: self.column,
              namespace: vec![],
              name: identifier,
            },
          };
          return Some(Ok(token));
        }
        // String literal
        Some('"') => {
          let mut string_raw_content = String::new();
          loop {
            match self.next_skip_char(
              char_is_not_double_quote,
              Some(&mut string_raw_content),
            ) {
              None => {
                return Some(Err(LexerError::UnexpectedLiteralStringEnding {
                  line: self.line,
                  column: self.column,
                }))
              }
              Some('"') => {}
              Some(c) => {
                return Some(Err(LexerError::InvalidState {
                  line: self.line,
                  column: self.column,
                  ch: Some(c),
                }))
              }
            }
            // Count the number of \ before the "
            // If it's odd, then this " is escaped and doesn't count as the end
            let mut backslash_count = 0;
            for c in string_raw_content.chars().rev() {
              if c == '\\' {
                backslash_count += 1;
              } else {
                break;
              }
            }
            if backslash_count % 2 == 0 {
              // replace escaped characters
              let mut strcontent = String::new();
              string_raw_content.chars().fold(
                false,
                |escaped, c| match escaped {
                  true => {
                    strcontent.push(c);
                    false
                  }
                  false => match c {
                    '\\' => true,
                    c => {
                      strcontent.push(c);
                      false
                    }
                  },
                },
              );
              return Some(Ok(Token::LiteralString {
                line: self.line,
                column: self.column,
                value: strcontent,
              }));
            } else {
              string_raw_content.push('"');
            }
          }
        }
        // Unknown character
        Some(c) => {
          return Some(Err(LexerError::InvalidState {
            line: self.line,
            column: self.column,
            ch: Some(c),
          }))
        }
      }
    }
  }
}
