#[derive(Debug, Clone, PartialEq)]
pub enum ParserError {
  ExpectedOneOf { expectations: Vec<String> },
  ExpectedUnqualifiedName,
  ExpectedTypedName,
  ExpectedFunctionDef,
  ExpectedArgListEnd,
  ExpectedIndentation,
}
