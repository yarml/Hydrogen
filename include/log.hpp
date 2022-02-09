#ifndef HYC_LOG_HPP
#define HYC_LOG_HPP

#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <token.hpp>

// This file is used to support defining operator<< for objects desired to be formatted

inline char const* TOKEN_TYPES[] = {
    // free fields
            "NUM","CHR",
            "STR","ID",
            // set fields
            // -- Keywords
            "PRINT", // Just for debugging not part of Hydrogen
            // --- declarations
            "DEF","DECL","PUBLIC","PROTECTED","PRIVATE","GLOBAL","LOCAL",
            "VAR","CONST","FUNC","CLASS","RETURNS","ACCESS","NAMESPACE",
            // --- Statements
            "BREAK","CONTINUE","RETURN","IF","ELIF","ELSE","FOR",
            "IN","FOREACH","WHILE","DO","IMPORT","FROM","BOOL",
            // -- Separators
            "SEP", "COLON",
            "COMMA", "NS_SEP",
            // -- Operators
            // --- Assignment
            // ---- Algebric assignment
            "ASSIGN", "ASSIGN_ADD", "ASSIGN_SUB", "ASSIGN_MUL",
            "ASSIGN_DIV", "ASSIGN_MOD", "INCR", "DECR",
            // ---- bit manip assignment
            "ASSIGN_SHIFTL", "ASSIGN_SHIFTR", 
            "ASSIGN_AND", "ASSIGN_OR", "ASSIGN_XOR",
            // --- Comparaison
            "EQUAL", "NOT_EQUAL", "GREATER", 
            "LESS", "GREAT_EQ", "LESS_EQ",
            // --- bit manip
            "AND", "OR", "NOT",
            "SHIFTL", "SHIFTR",
            "BIT_AND", "BIT_OR", "BIT_XOR", "BIT_NOT",
            // --- Algebric
            "ADD", "SUB", "MUL", "DIV", "MOD",
            // - Brackets
            "PARENL", "PARENR",
            "BRACEL", "BRACER",
            "BRACKL", "BRACKR",
            // special
            "INDENT", "DEDENT", "TOK_EOF"
};

template<>
struct fmt::formatter<hyc::lex::token::type>
{
    constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end();
    }
    template <typename FormatContext>
    auto format(const hyc::lex::token::type& input, FormatContext& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(),
            "{}",
            TOKEN_TYPES[(int) input]);
    }
};



#endif
