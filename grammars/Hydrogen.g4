grammar Hydrogen;


// ---------- Parser ----------

source: stmt+ EOF;

stmt:
    dataholder_def STMT_SEP
  | print_stmt STMT_SEP
  | STMT_SEP;

dataholder_def: DEF scope_spec? type_spec ID ( ASSIGN expr)?;
print_stmt: PRINT expr;

expr:
    NUM
  | STR
  | ID
  | PARENL expr PARENR
  | expr BINOP expr
  | PREUNIOP expr
  | UNIOP expr
  | expr UNIOP
  ;


scope_spec: GLOBAL | LOCAL;
type_spec: dataholder_type_spec? type_id;
dataholder_type_spec: VAR | CONST;
type_id: ID | ID '<' type_spec '>';

// ---------- Lexer ------------

// -Debug
PRINT: 'print';
// -Keywords
DEF    : 'def'    ;
DECL   : 'decl'   ;
GLOBAL : 'global' ;
LOCAL  : 'local'  ;
VAR    : 'var'    ;
CONST  : 'const'  ;
FUNC   : 'func'   ;
RETURN : 'return' ;
RETURNS: 'returns';
IF     : 'if'     ;
ELIF   : 'elif'   ;
ELSE   : 'else'   ;
FOR    : 'for'    ;
FOREACH: 'foreach';
WHILE  : 'while'  ;
DO     : 'do'     ;
IN     : 'in'     ;
IMPORT : 'import' ;
FROM   : 'from'   ;

// -Statement separator
STMT_SEP: '\n' | ';' /*| EOF*/;
// -Operators
// --Assignment
// ---Algebric assignement
ASSIGN    : '=' ;
fragment ASSIGN_ADD: '+=';
fragment ASSIGN_SUB: '-=';
fragment ASSIGN_MUL: '*=';
fragment ASSIGN_DIV: '/=';
fragment ASSIGN_MOD: '%=';
fragment INCR      : '++'; // uniop
fragment DECR      : '--'; // uniop
// ---Bit manipulation assignement
fragment ASSIGN_SHIFTL: '<<=';
fragment ASSIGN_SHIFTR: '>>=';
fragment ASSIGN_AND   : '&=' ;
fragment ASSIGN_OR    : '|=' ;
fragment ASSIGN_XOR   : '^=' ;
fragment ASSIGN_NOT   : '~=' ;
// --Comparaison
fragment EQUAL     : '==';
fragment NOT_EQUAL : '!=';
fragment GREATER   : '>' ;
fragment LESSER    : '<' ;
fragment GREATER_EQ: '>=';
fragment LESSER_EQ : '<=';
// --bit manipulation
fragment SHIFTL : '<<' ;
fragment SHIFTR : '>>' ;
fragment AND    : 'and';
fragment OR     : 'or' ;
fragment XOR    : 'xor';
fragment NOT    : 'not'; // preuniop
fragment BIT_AND: '&'  ;
fragment BIT_OR : '|'  ;
fragment BIT_XOR: '^'  ;
fragment BIT_NOT: '~'  ; // preuniop
// --Algebric operators
fragment ADD: '+';
fragment SUB: '-';
fragment MUL: '*';
fragment DIV: '/';
fragment MOD: '%';
// --
BINOP: 
    ASSIGN | ASSIGN_ADD | ASSIGN_SUB | ASSIGN_MUL | ASSIGN_DIV | ASSIGN_MOD
  | ASSIGN_SHIFTL | ASSIGN_SHIFTR | ASSIGN_AND | ASSIGN_OR | ASSIGN_XOR | ASSIGN_NOT
  | EQUAL | NOT_EQUAL | GREATER | LESSER | GREATER_EQ | LESSER_EQ
  | SHIFTL | SHIFTR | AND | OR | XOR | BIT_AND | BIT_OR | BIT_XOR
  | ADD | SUB | MUL | DIV | MOD
  ;

PREUNIOP: NOT | BIT_NOT | ADD | SUB;
UNIOP   : INCR | DECR              ;
// -Brackets
PARENL: '(';
PARENR: ')';
BRACEL: '{';
BRACER: '}';
BRACKL: '[';
BRACKR: ']';
// -User specified text
fragment DEC: ([0-9] [0-9_]*)     ;
fragment BIN: '0b' ([01_]+)       ;
fragment OCT: '0c' ([0-7_]+)      ;
fragment HEX: '0x' ([0-9A-Fa-f_]+);

fragment INT  : DEC | BIN | OCT | HEX;
fragment FLOAT: DEC? '.' DEC         ;

NUM: FLOAT | INT;

ID    : [a-zA-Z_] [a-zA-Z0-9_]*;

fragment VCHAR: ('\\' . | . );

STR: '\'' VCHAR*? '\'';
CHR: '"'  VCHAR   '"' ;

WS: [ \t] -> skip;