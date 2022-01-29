parser grammar HydrogenParser;

options
{
  tokenVocab=HydrogenLexer;
}


source: decl_seq EOF;

sep: SEP+;

decl_seq: (decl sep)* decl sep;

decl:
  attribute_seq?
  ( namespace_decl
  | DECL class_decl
  | DEF  class_def
  | DECL func_decl
  | DEF  func_def
  | DECL strg_decl
  | DEF  strg_def
);



// namespace
namespace_decl: NAMESPACE qualified_id COLON INDENT decl_seq DEDENT;
// Classes
class_decl: class_sig;
class_def : class_sig COLON class_block;
class_sig: CLASS class_id;
class_block:
    INDENT class_strg_decl_seq DEDENT
  | class_strg_decl;
class_strg_decl_seq: (class_strg_decl sep)* class_strg_decl;
class_strg_decl: class_member_visibility? strg_type? type_id unqualified_id;

// Functions
func_decl: func_sig;
func_def: func_sig COLON stmt_block;
func_sig: id_scope? FUNC func_id PARENL func_arg_decl_seq PARENR (class_member_visibility? IN type_id)? (RETURNS type_id)?;
func_arg_decl_seq: (strg_sig COMMA)* strg_sig?;

func_call: qualified_id PARENL func_arg_seq PARENR;
func_arg_seq: (expression COMMA)* expression;

// Storage
strg_decl: strg_sig;
strg_def: strg_sig (ASSIGN expression)?;
strg_sig: id_scope? strg_type? type_id unqualified_id;

// Statements
stmt_block: INDENT stmt_seq DEDENT | stmt;

stmt_seq: (stmt sep)* stmt;

print_stmt: PRINT expression;

ret_stmt: RETURN expression;

if_stmt: IF expression COLON stmt_block elif_stmt* else_stmt?;
elif_stmt: ELIF expression COLON stmt_block;
else_stmt: ELSE COLON stmt_block;

while_stmt: WHILE expression COLON stmt_block;

stmt: 
    expression
  | print_stmt
  | ret_stmt
  | if_stmt
  | while_stmt;


// Expressions
// expression: expression_or (assignment_op expression_or)*;

// expression_or      : expression_and      (OR          expression_and     )*;
// expression_and     : expression_bitor    (AND         expression_bitor   )*;
// expression_bitor   : expression_bitxor   (BIT_OR      expression_bitxor  )*;
// expression_bitxor  : expression_bitand   (BIT_XOR     expression_bitand  )*;
// expression_bitand  : expression_equality (BIT_AND     expression_equality)*;
// expression_equality: expression_rel_comp (equality_op expression_rel_comp)*;
// expression_rel_comp: expression_bitshift (rel_comp_op expression_bitshift)*;
// expression_bitshift: expression_sum      (bitshift_op expression_sum     )*;
// expression_sum     : expression_product  (sum_op      expression_product )*;
// expression_product : expression_prefix   (prod_op     expression_prefix  )*;
// expression_prefix  : expression_postfix | prefix_op expression_prefix      ;
// expression_postfix : expression_primary | expression_postfix postfix_op    ;
// expression_primary :
//     literal
//   | PARENL expression PARENR
//   | qualified_id
//   | func_call;

expression:
  // atomic expressions
    literal                  // can be int, float, bool, char or string
  | PARENL expression PARENR // pointer to expression
  | qualified_id             // identifier
  | func_call                // identifer + list<expr>
  // all operator expressions who's one side isn't a builtin type are function calls
  // unary expressions // operator + position + pointer to expression
  | expression postfix_op
  | prefix_op expression
  // binary expressions // operator + pointer to left expression + pointer to right expression
  | expression prod_op     expression
  | expression sum_op      expression
  | expression bitshift_op expression
  | expression rel_comp_op expression
  | expression equality_op expression
  | expression BIT_AND     expression
  | expression BIT_XOR     expression
  | expression BIT_OR      expression
  | expression AND         expression
  | expression OR          expression;

// Attributes

attribute_seq: (attribute sep)* attribute sep;
attribute: BRACKL BRACKL attribute_id BRACKR BRACKR;


// ------------- Extended Lexer ---------------------
// class
class_id: unqualified_id;
class_member_visibility: PUBLIC | PROTECTED | PRIVATE;
// func
func_id: ID;
// strg
strg_type: VAR | CONST;
strg_id: ID;
// ids
type_id: qualified_id;

qualified_id: global_namespace=NAMESPACE_SEP? unqualified_id? (NAMESPACE_SEP unqualified_id)*;
unqualified_id: ID;

id_scope: GLOBAL | LOCAL;
// expressions
assignment_op:
    ASSIGN
  | ASSIGN_ADD | ASSIGN_SUB
  | ASSIGN_MUL | ASSIGN_DIV | ASSIGN_MOD
  | ASSIGN_SHIFTL | ASSIGN_SHIFTR 
  | ASSIGN_AND | ASSIGN_OR | ASSIGN_XOR;
equality_op: EQUAL | NOT_EQUAL;
rel_comp_op: GREATER | LESS | GREAT_EQ | LESS_EQ;
bitshift_op: SHIFTL | SHIFTR;
sum_op     : ADD | SUB;
prod_op    : MUL | DIV | MOD;
prefix_op  : ADD | SUB | INCR | DECR;
postfix_op : INCR | DECR;
literal: INT | FLOAT | BOOL | CHR | STR;
// attribute
attribute_id: ID;
