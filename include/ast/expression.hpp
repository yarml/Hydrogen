#pragma once

#include <ast/node.hpp>
#include <ast/identifier.hpp>
#include <variant>
#include <vector>

namespace hyc::ast
{
    enum class expr_type { ATOMIC, UNARY, BINARY };
    enum class atomic_expr_type { LITERAL, EXPR, IDENTIFIER, FUNC_CALL };

    struct expr : public node
    {
        expr_type type;
    };

    struct atomic_expr : public expr
    {
        atomic_expr_type atom_type;
    };

    using literal_type = std::variant<int, float, bool, char, std::string>;
    struct literal : public atomic_expr
    {
        literal_type data;
    };

    struct id_expr : public atomic_expr
    {
        qualified_id_ptr identifier;
    };

    struct func_call : public atomic_expr
    {
        qualified_id_ptr  func_id;
        std::vector<expr> args   ;
    };

    using expr_ptr = std::unique_ptr<expr>;
    using atomix_expr_ptr = std::unique_ptr<atomic_expr>;
    using literal_ptr = std::unique_ptr<literal>;
    using id_expr_ptr = std::unique_ptr<id_expr>;
    using func_call_ptr = std::unique_ptr<func_call>;
}
