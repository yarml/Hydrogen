use crate::lexer::token::Token;

use super::{
  error::ParserError,
  nodes::{QualifiedName, TypedName},
};

pub fn parse_typed_name(
  remaining_tokens: &[Token],
) -> Result<(TypedName, &[Token]), ParserError> {
  match remaining_tokens {
    [Token::Identifier {
      namespace, name, ..
    }, Token::Colon { .. }, Token::Identifier {
      namespace: type_namespace,
      name: type_name,
      ..
    }, rest @ ..]
      if namespace.len() == 0 =>
    {
      Ok((
        TypedName {
          name: name.clone(),
          type_name: QualifiedName {
            namespaces: type_namespace.clone(),
            name: type_name.clone(),
          },
        },
        rest,
      ))
    }
    _ => Err(ParserError::ExpectedTypedName),
  }
}

pub fn parse_arglist(
  mut remaining_tokens: &[Token],
) -> Result<(Vec<TypedName>, &[Token]), ParserError> {
  let mut args: Vec<TypedName> = Vec::new();
  loop {
    match parse_typed_name(remaining_tokens) {
      Ok((name, rest)) => {
        remaining_tokens = rest;
        args.push(name);
        match remaining_tokens[0] {
          Token::Colon { .. } => remaining_tokens = &remaining_tokens[1..],
          _ => return Ok((args, remaining_tokens)),
        }
      }
      Err(_) => {
        return Ok((args, remaining_tokens));
      }
    }
  }
}
