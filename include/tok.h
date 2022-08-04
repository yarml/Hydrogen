#ifndef HYC_TOK_H
#define HYC_TOK_H

#include <stddef.h>

enum HYC_TOKEN_TYPE
{
    // free fields
    TOK_NUM, TOK_CHR,
    TOK_STR, TOK_ID,
    // set fields
    // -- Keywords
    TOK_PRINT, // Just for debugging not part of Hydrogen
    // --- declarations
    TOK_DEF,TOK_DECL,TOK_PUBLIC,TOK_PROTECTED,TOK_PRIVATE,TOK_GLOBAL,TOK_LOCAL,
    TOK_VAR,TOK_CONST,TOK_FUNC,TOK_CLASS,TOK_RETURNS,TOK_ACCESS,TOK_NAMESPACE,
    // --- Statements
    TOK_BREAK,TOK_CONTINUE,TOK_RETURN,TOK_IF,TOK_ELIF,TOK_ELSE,TOK_FOR,
    TOK_IN,TOK_FOREACH,TOK_WHILE,TOK_DO,TOK_IMPORT,TOK_FROM,TOK_BOOL,
    // -- Separators
    TOK_SEP, TOK_COLON,
    TOK_COMMA, TOK_NS_SEP,
    // -- Operators
    // --- Assignment
    // ---- Algebric assignment
    TOK_ASSIGN, TOK_ASSIGN_ADD, TOK_ASSIGN_SUB, TOK_ASSIGN_MUL,
    TOK_ASSIGN_DIV, TOK_ASSIGN_MOD, TOK_INCR, TOK_DECR,
    // ---- bit manip assignment
    TOK_ASSIGN_SHIFTL, TOK_ASSIGN_SHIFTR, 
    TOK_ASSIGN_AND, TOK_ASSIGN_OR, TOK_ASSIGN_XOR,
    // --- Comparaison
    TOK_EQUAL, TOK_NOT_EQUAL, TOK_GREATER, 
    TOK_LESS, TOK_GREAT_EQ, TOK_LESS_EQ,
    // --- bit manip
    TOK_AND, TOK_OR, TOK_NOT,
    TOK_SHIFTL, TOK_SHIFTR,
    TOK_BIT_AND, TOK_BIT_OR, TOK_BIT_XOR, TOK_BIT_NOT,
    // --- Algebric
    TOK_ADD, TOK_SUB, TOK_MUL, TOK_DIV, TOK_MOD,
    // - Brackets
    TOK_PARENL, TOK_PARENR,
    TOK_BRACEL, TOK_BRACER,
    TOK_BRACKL, TOK_BRACKR,
    // special
    TOK_INDENT, TOK_DEDENT, TOK_EOF
};

typedef enum HYC_TOKEN_TYPE tok_type;

struct HYC_TOK
{
    size_t   line;
    size_t   colm;
    tok_type type;
    char*    text;

    void*    more; // points to a struct depending on the tok_type
};

typedef struct HYC_TOK token;

#endif