parser grammar HydrogenParser;

options
{
  language=Cpp;
  tokenVocab=HydrogenLexer;
}


source: decl_seq EOF;

decl_seq: (decl SEP)* decl;

decl:
    class_decl
  | func_decl
  | strg_decl;

// Classes
class_decl:
    attribute_seq? DECL class_sig
  | attribute_seq? DEF  class_sig COLON class_block;
class_sig: CLASS class_id;
class_block:
    INDENT class_strg_decl_seq DEDENT
  | class_strg_decl;
class_strg_decl_seq: (class_strg_decl SEP)* class_strg_decl;
class_strg_decl: class_member_visibility? class_strg_type? ; // TODO: not completed!

// Functions
func_decl:
    attribute_seq? DECL func_sig
  | attribute_seq? DEF  func_sig COLON stmt_block;
func_sig: func_scope FUNC func_id PARENL func_arg_seq PARENR;
func_arg_seq: (func_arg COMMA)* func_arg;
func_arg: func_arg_type?; // TODO: not completed!


// Storage
strg_decl: 
    attribute_seq? DECL strg_sig
  | attribute_seq? DEF  strg_sig;
strg_sig: ; // TODO: not completed!

// Statements
stmt_block: (stmt SEP)* stmt;
stmt: ;

// Expressions
expression: expression_or (assignment_op expression_or)*;

expression_or      : expression_and      (OR          expression_and     )*;
expression_and     : expression_bitor    (AND         expression_bitor   )*;
expression_bitor   : expression_bitxor   (BIT_OR      expression_bitxor  )*;
expression_bitxor  : expression_bitand   (BIT_XOR     expression_bitand  )*;
expression_bitand  : expression_equality (BIT_AND     expression_equality)*;
expression_equality: expression_rel_comp (equality_op expression_rel_comp)*;
expression_rel_comp: expression_bitshift (rel_comp_op expression_bitshift)*;
expression_bitshift: expression_sum      (bitshift_op expression_sum     )*;
expression_sum     : expression_product  (sum_op      expression_product )*;
expression_product : expression_prefix   (prod_op     expression_prefix  )*;
expression_prefix  : expression_postfix | prefix_op expression_prefix      ;
expression_postfix : expression_primary | expression_postfix postfix_op    ;
expression_primary :
    literal
  | PARENL expression PARENR
  | expression_id;
expression_id: ID;

// Attributes

attribute_seq: (attribute SEP)* attribute;
attribute: BRACKL BRACKL attribute_id BRACKR BRACKR;


// ------------- Extended Lexer ---------------------
// class
class_id: ID;
class_member_visibility: PUBLIC | PROTECTED | PRIVATE;
class_strg_type: VAR | CONST;
// func
func_id: ID;
func_scope: GLOBAL | LOCAL;
func_arg_type: VAR | CONST;
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
