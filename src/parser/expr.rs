use crate::lexer::token::Token;

use super::{nodes::QualifiedName, ParserError};

#[derive(Debug, Clone, PartialEq)]
pub enum Expression {
  AtomString {
    value: String,
  },
  AtomInteger {
    value: i32,
  },
  AtomFloat {
    value: f32,
  },

  Add {
    left: Box<Expression>,
    right: Box<Expression>,
  },
  Sub {
    left: Box<Expression>,
    right: Box<Expression>,
  },
  Mul {
    left: Box<Expression>,
    right: Box<Expression>,
  },
  Div {
    left: Box<Expression>,
    right: Box<Expression>,
  },

  FunctionCall {
    name: QualifiedName,
    args: Vec<Expression>,
  },
}

pub fn parse_expression(
  mut remaining_tokens: &[Token],
) -> Result<(Expression, &[Token]), ParserError> {
  match remaining_tokens {
    [Token::LiteralString { value, .. }, rest @ ..] => {
      return Ok((
        Expression::AtomString {
          value: value.clone(),
        },
        rest,
      ));
    }
    [Token::Identifier {
      namespace,
      name: func_name,
      ..
    }, Token::ParenOpen { .. }, rest @ ..] => {
      remaining_tokens = rest;
      let mut expressions = Vec::new();
      loop {
        match remaining_tokens {
          [Token::ParenClose { .. }] => break,
          _ => {}
        }
        match parse_expression(remaining_tokens)? {
          (expr, rest) => {
            remaining_tokens = rest;
            expressions.push(expr);
          }
        }

        match remaining_tokens {
          [Token::Comma { .. }, rest @ ..] => {
            remaining_tokens = rest;
          }
          _ => break,
        }
      }

      return match remaining_tokens {
        [Token::ParenClose { .. }, rest @ ..] => Ok((
          Expression::FunctionCall {
            name: QualifiedName {
              namespaces: namespace.clone(),
              name: func_name.clone(),
            },
            args: expressions,
          },
          rest,
        )),
        _ => Err(ParserError::ExpectedOneOf {
          expectations: vec![")".into()],
        }),
      };
    }
    _ => todo!(),
  }
}
