#pragma once

#include <ast/node.hpp>
#include <ast/identifier.hpp>
#include <vector>

namespace hyc::ast
{
    enum class expr_type { ATOMIC, UNARY, BINARY };
    enum class atomic_expr_type { LITERAL, EXPR, IDENTIFIER, FUNC_CALL };
    enum class literal_expr_type { INT, FLOAT, BOOL, CHAR, STR };

    struct expr : public node
    {
        expr_type type;
    };

    using expr_ptr = std::unique_ptr<expr>;
    
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
