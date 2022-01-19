lexer grammar HydrogenLexer;

options
{
    language=Cpp;
    superClass=hydrogen_lexer_base;
}

@header
{
#include <hydrogen_lexer_base.hpp>
}

tokens { INDENT, DEDENT }

// -Keywords
// --Debug
PRINT    : 'print';
// --Standard
DEF      : 'def'      ;
DECL     : 'decl'     ;
PUBLIC   : 'public'   ;
PROTECTED: 'protected';
PRIVATE  : 'private'  ;
GLOBAL   : 'global'   ;
LOCAL    : 'local'    ;
VAR      : 'var'      ;
CONST    : 'const'    ;
FUNC     : 'func'     ;
CLASS    : 'class'    ;
RETURN   : 'return'   ;
RETURNS  : 'returns'  ;
IF       : 'if'       ;
ELIF     : 'elif'     ;
ELSE     : 'else'     ;
FOR      : 'for'      ;
FOREACH  : 'foreach'  ;
WHILE    : 'while'    ;
DO       : 'do'       ;
IN       : 'in'       ;
IMPORT   : 'import'   ;
FROM     : 'from'     ;

// -Statement separator
SEP  : '\n' | ';';
COLON: ':'       ;
COMMA: ','       ;
// -Operators
// --Assignment
// ---Algebric assignement
ASSIGN    : '=' ;
ASSIGN_ADD: '+=';
ASSIGN_SUB: '-=';
ASSIGN_MUL: '*=';
ASSIGN_DIV: '/=';
ASSIGN_MOD: '%=';
INCR      : '++'; // uniop
DECR      : '--'; // uniop
// ---Bit manipulation assignement
ASSIGN_SHIFTL: '<<=';
ASSIGN_SHIFTR: '>>=';
ASSIGN_AND   : '&=' ;
ASSIGN_OR    : '|=' ;
ASSIGN_XOR   : '^=' ;
// --Comparaison
EQUAL     : '==';
NOT_EQUAL : '!=';
GREATER   : '>' ;
LESS      : '<' ;
GREAT_EQ  : '>=';
LESS_EQ   : '<=';
// --bit manipulation
SHIFTL : '<<' ;
SHIFTR : '>>' ;
AND    : 'and';
OR     : 'or' ;
NOT    : 'not'; // preuniop
BIT_AND: '&'  ;
BIT_OR : '|'  ;
BIT_XOR: '^'  ;
BIT_NOT: '~'  ; // preuniop
// --Algebric operators
ADD: '+';
SUB: '-';
MUL: '*';
DIV: '/';
MOD: '%';
// -Brackets
PARENL: '(';
PARENR: ')';
BRACEL: '{';
BRACER: '}';
BRACKL: '[';
BRACKR: ']';
// -User specified text
// -- Booleans
fragment TRUE : 'true' ;
fragment FALSE: 'false';
BOOL: TRUE | FALSE;
// -- Numbers
fragment DEC:       [0-9_]      +;
fragment BIN: '0b'  [01_]       +;
fragment OCT: '0c'  [0-7_]      +;
fragment HEX: '0x'  [0-9A-Fa-f_]+;

fragment INT_SUFFIX  : [iI];
fragment FLOAT_SUFFIX: [fF];

INT  : (DEC | BIN | OCT | HEX) INT_SUFFIX  ?;
FLOAT: (DEC? '.' DEC         ) FLOAT_SUFFIX?;
// -- identifiers
ID    : [a-zA-Z_] [a-zA-Z0-9_]*;

// -- Strings
fragment VCHAR: ('\\' . | . );
STR: '"'  VCHAR*? '"'  ;
CHR: '\'' VCHAR   '\'' ;

// -- Ignore
WS: [ \t] -> skip;
CMT: ('#' .*? '\n' | '##' .*? '##') -> skip;
