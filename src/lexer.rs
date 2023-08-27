pub mod error;
pub mod token;

mod next_token;
mod utils;

use queues::{IsQueue, Queue};
use std::str::Chars;

use self::{error::LexerError, token::Token};

pub struct Lexer<'source> {
  head: Chars<'source>,
  putback: Queue<char>,
  next_buf: Queue<Token>,

  line: usize,
  column: usize,
  line_lengths: Vec<usize>,

  indentation: usize,
}

type TokenResult = Result<Token, LexerError>;

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
  //   for c in seq.chars() {
  //     self.putbackc(c);
  //   }
  // }

  fn putbackc(&mut self, c: char) {
    self.putback.add(c).unwrap();

    match c {
      '\n' => {
        self.line -= 1;
        self.column = self.line_lengths[self.line - 1];
      }
      _ => self.column -= 1,
    }
  }

  pub fn check(
    lexed_source: Vec<Result<Token, LexerError>>,
  ) -> Option<Vec<Token>> {
    let mut checked_source = Vec::new();
    for result in lexed_source {
      if result.is_err() {
        let error = result.unwrap_err().error_str();
        println!("Syntax error:");
        println!("{}", error);
        return None;
      } else {
        checked_source.push(result.unwrap());
      }
    }
    Some(checked_source)
  }
}

impl<'source> Iterator for Lexer<'source> {
  type Item = TokenResult;

  fn next(&mut self) -> Option<Self::Item> {
    if self.next_buf.size() > 0 {
      return Some(Ok(self.next_buf.remove().unwrap()));
    }

    let real_token = self.next_real_token();
    // Return missing dedents if end of file
    if real_token.is_none() && self.indentation > 0 {
      self.indentation -= 1;
      return Some(Ok(Token::Dedent {
        line: self.line,
        column: self.column,
      }));
    } else {
      return real_token;
    }
  }
}
