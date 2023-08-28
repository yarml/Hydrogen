use crate::lexer::error::LexerError;

use super::{
  token::Token,
  utils::{char_is_not_double_quote, char_is_whitespace_not_newline},
  Lexer,
};

impl<'source> Lexer<'source> {
  pub(super) fn next_real_token(
    &mut self,
  ) -> Result<Option<Token>, LexerError> {
    loop {
      match self.next_skip_char(char_is_whitespace_not_newline, None) {
        // End of file
        None => return Ok(None),
        // Comments
        Some('#') => match self.next_char() {
          None => return Ok(None),
          Some('\n') => self.putbackc('\n'),
          Some('#') => match self.next_until("##") {
            None => return Ok(None),
            Some(c) => self.putbackc(c),
          },
          Some(_) => match self.next_until("\n") {
            None => return Ok(None),
            Some(c) => {
              self.putbackc('\n');
              self.putbackc(c);
            }
          },
        },
        // One character symbols
        Some('(') => {
          return Ok(Some(Token::ParenOpen {
            location: self.current_location(),
          }))
        }
        Some(')') => {
          return Ok(Some(Token::ParenClose {
            location: self.current_location(),
          }))
        }
        Some(':') => {
          return Ok(Some(Token::Colon {
            location: self.current_location(),
          }))
        }
        Some(',') => {
          return Ok(Some(Token::Comma {
            location: self.current_location(),
          }))
        }
        // Indents and dedents
        Some('\n') => {
          let lbeg_location = self.current_location();
          let mut spaces = String::new();
          match self
            .next_skip_char(char_is_whitespace_not_newline, Some(&mut spaces))
          {
            None => return Ok(None),
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
                  return Err(LexerError::UnknownIndentationType {
                    location: lbeg_location,
                  })
                }
              };
              let mut indentation = 0;
              for space in spaces.chars() {
                if !deduced_indent_type.match_char(space) {
                  return Err(LexerError::MixSpacesAndTabsIndentations {
                    location: lbeg_location,
                  });
                }

                indentation += 1;
              }

              match deduced_indent_type {
                IndentType::Space if indentation % 2 != 0 => {
                  return Err(LexerError::OddSpaceIndentation {
                    location: lbeg_location,
                  })
                }
                IndentType::Space => indentation /= 2,
                _ => {}
              }

              let old_indentation = self.indentation;
              self.indentation = indentation;

              match indentation as isize - old_indentation as isize {
                pdiff if pdiff > 1 => {
                  return Err(LexerError::MultipleIndentations {
                    location: lbeg_location,
                  })
                }
                1 => {
                  return Ok(Some(Token::Indent {
                    location: lbeg_location,
                  }))
                }
                0 => {}
                ndiff => {
                  for _ in 0..-((ndiff - 1) as isize) {
                    self.next_buf.push_back(Token::Dedent {
                      location: lbeg_location.clone(),
                    });
                  }
                  return Ok(Some(Token::Dedent {
                    location: lbeg_location,
                  }));
                }
              }
            }
          }
        }
        // Identifiers & keywords
        Some(c) if c.is_alphabetic() => {
          let word_location = self.current_location();
          let mut identifier = format!("{c}");
          let next =
            self.next_skip_char(char::is_alphanumeric, Some(&mut identifier));
          if next.is_some() {
            self.putbackc(next.unwrap());
          }
          let token = match identifier.as_str() {
            "func" => Token::KeywordFunc {
              location: word_location,
            },
            "def" => Token::KeywordDef {
              location: word_location,
            },
            _ => Token::Identifier {
              location: word_location,
              namespace: vec![],
              name: identifier,
            },
          };
          return Ok(Some(token));
        }
        // String literal
        Some('"') => {
          let str_location = self.current_location();
          let mut string_raw_content = String::new();
          loop {
            match self.next_skip_char(
              char_is_not_double_quote,
              Some(&mut string_raw_content),
            ) {
              None => {
                return Err(LexerError::UnexpectedLiteralStringEnding {
                  beg_location: str_location,
                  end_location: self.current_location(),
                })
              }
              Some('"') => {}
              Some(_) => {
                return Err(LexerError::InvalidState {
                  location: self.current_location(),
                })
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
              return Ok(Some(Token::LiteralString {
                location: str_location,
                value: strcontent,
              }));
            } else {
              string_raw_content.push('"');
            }
          }
        }
        // Unknown character
        Some(c) => {
          self.putbackc(c);
          return Err(LexerError::UnexpectedSymbol {
            location: self.current_location(),
          });
        }
      }
    }
  }
}
