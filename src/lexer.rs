use std::str::Chars;

use queues::{IsQueue, Queue};

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

  Indent {
    line: usize,
    column: usize,
  },
  Dedent {
    line: usize,
    column: usize,
  },
}

#[derive(Debug, Clone, PartialEq)]
pub enum LexerError {
  InvalidState { line: usize, column: usize },
  UnknownIndentationType { line: usize, column: usize },
  MixSpacesAndTabsIndentations { line: usize, column: usize },
  OddSpaceIndentation { line: usize, column: usize },
  MultipleIndentations { line: usize, column: usize },
}

pub struct Lexer<'source> {
  head: Chars<'source>,
  putback: Queue<char>,
  next_buf: Queue<Token>,

  line: usize,
  column: usize,
  line_lengths: Vec<usize>,

  indentation: usize,
}

impl<'source> Lexer<'source> {
  pub fn new(source: &'source String) -> Self {
    Self {
      head: source.chars(),
      putback: Queue::new(),
      next_buf: Queue::new(),
      line: 1,
      column: 1,
      line_lengths: Vec::new(),
      indentation: 0,
    }
  }

  fn next_skip_char<F: 'static + FnMut(char) -> bool>(
    &mut self,
    mut skip: F,
    mut buf: Option<&mut String>,
  ) -> Option<char> {
    loop {
      match self.next_char() {
        None => return None,
        Some(c) => {
          if !skip(c) {
            return Some(c);
          } else if buf.is_some() {
            let bufref = buf.unwrap();
            bufref.push(c);
            buf = Some(bufref);
          }
        }
      }
    }
  }

  fn next_char(&mut self) -> Option<char> {
    let c = if self.putback.size() > 0 {
      Some(self.putback.remove().unwrap())
    } else {
      self.head.next()
    };

    if c.is_some() {
      match c.unwrap() {
        '\n' => {
          self.line += 1;
          if self.line >= self.line_lengths.len() {
            self.line_lengths.push(self.column);
          }
          self.column = 1;
        }
        _ => self.column += 1,
      }
    }

    println!("-+* next_char() -> {:?}", c);
    c
  }

  fn next_until(&mut self, sequence: &str) -> Option<char> {
    match sequence.chars().nth(0) {
      None => self.next_char(),
      Some(first_c) => 'next_match: loop {
        match self.next_skip_char(move |c| c != first_c, None) {
          None => return None,
          Some(_) => {
            for c in sequence[1..].chars() {
              match self.head.next() {
                Some(ac) if ac == c => {}
                Some(ac) => {
                  if ac == first_c {
                    self.putbackc(ac);
                  }
                  continue 'next_match;
                }
                None => return None,
              }
            }
          }
        }
      },
    }
  }

  // fn putback(&mut self, seq: &str) {
  //   for c in seq.chars() {
  //     self.putbackc(c);
  //   }
  // }

  fn putbackc(&mut self, c: char) {
    println!("-+* putback({:?})", c);

    self.putback.add(c).unwrap();

    match c {
      '\n' => {
        self.line -= 1;
        self.column = self.line_lengths[self.line - 1];
      }
      _ => self.column -= 1,
    }
  }
}

impl<'source> Iterator for Lexer<'source> {
  type Item = Result<Token, LexerError>;

  fn next(&mut self) -> Option<Self::Item> {
    if self.next_buf.size() > 0 {
      return Some(Ok(self.next_buf.remove().unwrap()));
    }

    loop {
      match self.next_skip_char(char_is_whitespace_not_newline, None) {
        None => return None,
        Some(c) => match c {
          '#' => match self.next_char() {
            None => return None,
            Some(c) => match c {
              '\n' => self.putbackc('\n'),
              '#' => match self.next_until("##") {
                None => return None,
                Some(c) => self.putbackc(c),
              },
              _ => match self.next_until("\n") {
                None => return None,
                Some(c) => {
                  self.putbackc('\n');
                  self.putbackc(c);
                }
              },
            },
          },
          '\n' => {
            let mut spaces = String::new();
            match self
              .next_skip_char(char_is_whitespace_not_newline, Some(&mut spaces))
            {
              None => return None,
              Some(c) if c == '\n' => {
                self.putbackc('\n');
              }
              Some(c) => {
                println!("Bitch new line followed by {:?}", c);
                dbg!(&spaces);
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
                  Some(c) if c == ' ' => IndentType::Space,
                  Some(c) if c == '\t' => IndentType::Tab,
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
                    return Some(Err(
                      LexerError::MixSpacesAndTabsIndentations {
                        line: self.line,
                        column: 0,
                      },
                    ));
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

                println!(
                  "Indentation in line {} is {}",
                  self.line, self.indentation
                );

                match indentation - old_indentation {
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
          _ => {
            return Some(Err(LexerError::InvalidState {
              line: self.line,
              column: self.column,
            }))
          }
        },
      }
    }
  }
}

fn char_is_whitespace_not_newline(c: char) -> bool {
  c.is_whitespace() && c != '\n'
}
