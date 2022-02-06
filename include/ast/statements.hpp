#pragma once

#include <ast/expression.hpp>

namespace hyc::ast
{
    // INSTRUCTION is when a statement can be just one keyword, like 'break' or 'continue', those are instruction statements
    // SIMPLE is an instruction statetment that takes an expression, like 'return'
    // The rest are self explanatory
    enum class stmt_type { INSTRUCTION, SIMPLE, IF, WHILE, FOR, FOREACH, DO, IMPORT };
    enum class instruction_stmt_type { BREAK, CONTINUE, RETURN };
    enum class simple_stmt_type { RETURN, PRINT };

    struct stmt
    {
        stmt_type type;
    };

    struct instruction_stmt : public stmt
    {
        instruction_stmt_type ins_type;
    };

    struct simple_stmt : public stmt
    {
        simple_stmt_type smp_type;
        expr_ptr expr_arg;
    };
}
