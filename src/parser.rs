pub mod decl;
pub mod expr;
pub mod nodes;
pub mod parts;

pub mod error;

use std::process::exit;

use crate::lexer::token::Token;

use self::{
  decl::{parse_def_func, Declaration},
  error::ParserError,
  nodes::QualifiedName,
};

pub fn parse(
  source_tokens: &Vec<Token>,
) -> Result<Vec<Declaration>, ParserError> {
  let mut decls = Vec::new();
  let mut remaining_tokens = &source_tokens[..];
  loop {
    match remaining_tokens {
      [] => return Ok(decls),
      [Token::KeywordDef { .. }, rest @ ..] => match rest {
        [Token::KeywordFunc { .. }, rest @ ..] => match parse_def_func(rest)? {
          (decl, rest) => {
            decls.push(decl);
            remaining_tokens = rest
          }
        },
        _ => todo!(),
      },
      _ => {
        eprintln!("Could not parse ur giberrish");
        dbg!(source_tokens);
        dbg!(remaining_tokens);
        dbg!(decls);
        exit(0);
      }
    }
  }
}
