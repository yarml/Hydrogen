pub mod error;
pub mod location;
pub mod token;

mod next_token;
mod utils;

use std::{collections::VecDeque, process::exit, str::Chars};

use self::{location::TokenLocation, token::Token};

pub struct Lexer<'source> {
  head: Chars<'source>,
  putback: Vec<char>,
  next_buf: VecDeque<Token>,
  file_name: String,

  line: usize,
  column: usize,
  line_lengths: Vec<usize>,

  indentation: usize,
}

impl<'source> Lexer<'source> {
  pub fn new(file_name: String, source: &'source String) -> Self {
    Self {
      head: source.chars(),
      putback: Vec::new(),
      next_buf: VecDeque::new(),
      file_name,
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
    let c = if self.putback.len() > 0 {
      Some(self.putback.pop().unwrap())
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
              match self.next_char() {
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
            return self.next_char();
          }
        }
      },
    }
  }

  // fn putback(&mut self, seq: &str) {
  //   for c in seq.chars().rev() {
  //     self.putbackc(c);
  //   }
  // }

  fn putbackc(&mut self, c: char) {
    self.putback.push(c);

    match c {
      '\n' => {
        self.line -= 1;
        self.column = self.line_lengths[self.line - 1];
      }
      _ => self.column -= 1,
    }
  }

  fn current_location(&mut self) -> TokenLocation {
    let c = self.next_char();
    if c.is_some() {
      self.putbackc(c.unwrap())
    };
    TokenLocation::new(&self.file_name, self.line, self.column, c)
  }
}

impl<'source> Iterator for Lexer<'source> {
  type Item = Token;

  fn next(&mut self) -> Option<Self::Item> {
    if self.next_buf.len() > 0 {
      return self.next_buf.pop_front();
    }

    // Return missing dedents if end of file
    match self.next_real_token() {
      Err(err) => {
        let error_str = err.error_str();
        println!("Syntax error:");
        println!("{error_str}");
        exit(0);
      }
      Ok(None) if self.indentation > 0 => {
        self.indentation -= 1;
        return Some(Token::Dedent {
          location: self.current_location(),
        });
      }
      Ok(real_token) => return real_token,
    }
  }
}
