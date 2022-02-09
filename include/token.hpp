#ifndef HYC_TOK_HPP
#define HYC_TOK_HPP

#include <string>
#include <vector>

namespace hyc::lex
{
    class token
    {
    public:
        enum class type
        {
            // free fields
            NUM,CHR,
            STR,ID,
            // set fields
            // -- Keywords
            PRINT, // Just for debugging not part of Hydrogen
            // --- declarations
            DEF,DECL,PUBLIC,PROTECTED,PRIVATE,GLOBAL,LOCAL,
            VAR,CONST,FUNC,CLASS,RETURNS,ACCESS,NAMESPACE,
            // --- Statements
            BREAK,CONTINUE,RETURN,IF,ELIF,ELSE,FOR,
            IN,FOREACH,WHILE,DO,IMPORT,FROM,BOOL,
            // -- Separators
            SEP, COLON,
            COMMA, NS_SEP,
            // -- Operators
            // --- Assignment
            // ---- Algebric assignment
            ASSIGN, ASSIGN_ADD, ASSIGN_SUB, ASSIGN_MUL,
            ASSIGN_DIV, ASSIGN_MOD, INCR, DECR,
            // ---- bit manip assignment
            ASSIGN_SHIFTL, ASSIGN_SHIFTR, 
            ASSIGN_AND, ASSIGN_OR, ASSIGN_XOR,
            // --- Comparaison
            EQUAL, NOT_EQUAL, GREATER, 
            LESS, GREAT_EQ, LESS_EQ,
            // --- bit manip
            AND, OR, NOT,
            SHIFTL, SHIFTR,
            BIT_AND, BIT_OR, BIT_XOR, BIT_NOT,
            // --- Algebric
            ADD, SUB, MUL, DIV, MOD,
            // - Brackets
            PARENL, PARENR,
            BRACEL, BRACER,
            BRACKL, BRACKR,
            // special
            INDENT, DEDENT, TOK_EOF // fuck stdlib for defining EOF
        };
    public:
        token(
            std::string const& text,
            type               type,
            int                line,
            int                cpos);
    public:
        std::string const& get_text() const;
        std::string      & get_text()      ;
        type               get_type()      ;
        type               get_type() const;
        int                get_line() const;
        int                get_cpos() const;
    public:
        void set_text(std::string const& text);
        void set_type(type               type);
        void set_line(int                line);
        void set_cpos(int                cpos);
    private:
        std::string m_text;
        type        m_type;
        int         m_line;
        int         m_cpos;
    };
}

#endif