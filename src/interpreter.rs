use std::process::exit;

use crate::parser::{
  decl::Declaration, expr::Expression, nodes::QualifiedName,
};

pub struct Interpreter {
  decls: Vec<Declaration>,
}

impl Interpreter {
  pub fn new(decls: Vec<Declaration>) -> Self {
    Self { decls }
  }

  pub fn call_function(
    &self,
    name: &QualifiedName,
    args: &Vec<Expression>,
  ) -> Expression {
    // Check if its a builtin function
    match name {
      QualifiedName { namespaces, name }
        if namespaces.len() == 0 && name == "print" =>
      {
        args
          .iter()
          .for_each(|arg| print!("{}", arg.evaluate_as_str(self)));
        Expression::AtomNone
      }
      // I cba supporting namespaces now
      QualifiedName { namespaces, name } if namespaces.len() == 0 => {
        match self.decls.iter().find(|decl| match decl {
          Declaration::FunctionDef {
            name: decl_name, ..
          } if decl_name == name => true,
          _ => false,
        }) {
          Some(Declaration::FunctionDef { expressions, .. }) => {
            expressions.iter().for_each(|expr| {
              expr.evaluate_as_str(self);
            });
            expressions
              .iter()
              .rev()
              .nth(0)
              .unwrap_or(&Expression::AtomNone)
              .clone()
          }
          None => {
            eprintln!("Function {name} not found.");
            exit(1);
          }
        }
      }
      _ => todo!(),
    }
  }
}
