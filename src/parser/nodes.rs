#[derive(Debug, Clone, PartialEq)]
pub struct QualifiedName {
  pub namespaces: Vec<String>,
  pub name: String,
}

#[derive(Debug, Clone, PartialEq)]
pub struct TypedName {
  pub name: String,
  pub type_name: QualifiedName,
}

#[derive(Debug, Clone, PartialEq)]
pub struct TypedQualifiedName {
  pub name: QualifiedName,
  pub type_name: QualifiedName,
}
