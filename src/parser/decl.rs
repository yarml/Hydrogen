use crate::lexer::token::Token;

use super::{
  expr::{parse_expression, Expression},
  nodes::TypedName,
  parts::parse_arglist,
  ParserError, QualifiedName,
};

#[derive(Debug, Clone, PartialEq)]
pub enum Declaration {
  FunctionDef {
    name: String,
    args: Vec<TypedName>,
    return_type: QualifiedName,
    expressions: Vec<Expression>,
  },
}

pub fn parse_def_func(
  mut remaining_tokens: &[Token],
) -> Result<(Declaration, &[Token]), ParserError> {
  match remaining_tokens {
    [Token::Identifier {
      namespace,
      name: func_name,
      ..
    }, Token::ParenOpen { .. }, rest @ ..]
      if namespace.len() == 0 =>
    {
      match parse_arglist(rest)? {
        (func_args, rest) => match rest {
          [Token::ParenClose { .. }, Token::Colon { .. }, rest @ ..] => {
            let mut return_type = QualifiedName {
              name: "void".into(),
              namespaces: vec![],
            };

            match rest {
              [Token::Identifier {
                namespace, name, ..
              }, rest @ ..] => {
                remaining_tokens = rest;
                return_type = QualifiedName {
                  name: name.clone(),
                  namespaces: namespace.clone(),
                };
              }
              _ => remaining_tokens = rest,
            }
            match remaining_tokens {
              [Token::Indent { .. }, rest @ ..] => {
                remaining_tokens = rest;
                let mut expressions = Vec::new();
                loop {
                  match parse_expression(remaining_tokens)? {
                    (expression, rest) => {
                      remaining_tokens = rest;
                      expressions.push(expression);
                    }
                  }

                  match remaining_tokens {
                    [Token::Dedent { .. }, rest @ ..] => {
                      return Ok((
                        Declaration::FunctionDef {
                          name: func_name.into(),
                          args: func_args,
                          return_type,
                          expressions,
                        },
                        rest,
                      ))
                    }
                    _ => {}
                  }
                }
              }
              _ => Err(ParserError::ExpectedIndentation),
            }
          }
          _ => Err(ParserError::ExpectedArgListEnd),
        },
      }
    }
    _ => Err(ParserError::ExpectedFunctionDef),
  }
}
