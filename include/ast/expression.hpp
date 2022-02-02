#pragma once

#include <ast/node.hpp>
#include <ast/identifier.hpp>
#include <vector>

namespace hyc::ast
{
    enum class atomic_expr_type { LITERAL, IDENTIFIER, FUNC_CALL };
    enum class literal_expr_type { INT, FLOAT, BOOL, CHAR, STR };
    enum class unary_op { YES, INV, INCR, DECR };
    enum class unary_op_pos { BEFORE, AFTER };
    enum class binop
    { 
        ASSIGN, 
        ASSIGN_ADD, ASSIGN_SUB, 
        ASSIGN_MUL, ASSIGN_DIV, ASSIGN_MOD,
        ASSIGN_SHIFTL, ASSIGN_SHIFTR,
        ASSIGN_AND, ASSIGN_OR, ASSIGN_XOR,
        EQUAL, NOT_EQUAL,
        GREATER, LESS, GREAT_EQ, LESS_EQ,
        SHIFTL, SHIFTR,
        ADD, SUB,
        MUL, DIV, MOD
    };

    struct expr : public node
    {
    };

    using expr_ptr = std::unique_ptr<expr>;
    // composite expression expressions
    struct unary_expr : public expr
    {
        expr_ptr expr;
        unary_op_pos op_pos;
        unary_op op;
    };

    struct binary_expr : public expr
    {
        binop op;
    };

    using unary_expr_ptr = std::unique_ptr<unary_expr>;
    using binary_expr_ptr = std::unique_ptr<binary_expr>;

    // Atmoic expressions
    struct atomic_expr : public expr
    {
        atomic_expr_type atom_type;
    };

    struct literal : public atomic_expr
    {
        literal_expr_type literal_type;
    };

    struct int_literal : public literal
    {
        int val;
    };
    struct float_literal : public literal
    {
        float val;
    };
    struct bool_literal : public literal
    {
        bool val;
    };
    struct char_literal : public literal
    {
        char val;
    };
    struct str_literal : public literal
    {
        std::string val;
    };

    struct id_expr : public atomic_expr
    {
        qualified_id_ptr identifier;
    };

    
    struct func_call : public atomic_expr
    {
        qualified_id_ptr  func_id;
        std::vector<expr_ptr> args   ;
    };

    using atomix_expr_ptr = std::unique_ptr<atomic_expr>;
    using literal_ptr = std::unique_ptr<literal>;
    using int_literal_ptr = std::unique_ptr<int_literal>;
    using float_literal_ptr = std::unique_ptr<float_literal>;
    using bool_literal_ptr = std::unique_ptr<bool_literal>;
    using char_literal_ptr = std::unique_ptr<char_literal>;
    using str_literal_ptr = std::unique_ptr<str_literal>;
    using id_expr_ptr = std::unique_ptr<id_expr>;
    using func_call_ptr = std::unique_ptr<func_call>;
}
