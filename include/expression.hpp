#pragma once

#include <string>

namespace hydrogen::expression
{
    enum class binops
    {
        ASSIGN, ASSIGN_ADD, ASSIGN_SUB, ASSIGN_MUL, ASSIGN_DIV, ASSIGN_MOD,
        ASSIGN_SHIFTL, ASSIGN_SHIFTR, ASSIGN_AND, ASSIGN_OR, ASSIGN_XOR, ASSIGN_NOT,
        EQUAL, NOT_EQUAL, GREATER, LESSER, GREATER_EQ, LESSER_EQ,
        SHIFTL, SHIFTR, AND, OR, XOR, BIT_AND, BIT_OR, BIT_XOR,
        ADD, SUB, MUL, DIV, MOD
    };
    enum class uniops
    {
        NOT, BIT_NOT, ADD, SUB,
        INCR, DECR
    };

    extern binops str_to_binop(std::string const& str);
    extern uniops str_to_uniop(std::string const& str);
}
