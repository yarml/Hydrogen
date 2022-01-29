#pragma once

#include <ast/node.hpp>
#include <ast/identifier.hpp>
#include <ast/expression.hpp>

namespace hyc::ast
{
    enum class decl_type_e { DECL, DEF };
    enum class scope_type  { GLOBAL, LOCAL };
    enum class decl_class  { NAMESPACE, CLASS, FUNC, STRG };

    struct scope : public node
    {
        scope_type type;
    };
    

    struct decl_type : public node
    {
        decl_type_e type;
    };

    using scope_ptr = std::unique_ptr<scope>;
    using decl_type_ptr = std::unique_ptr<decl_type>;

    struct decl : public node
    {
        qualified_id_ptr id           ;
        decl_type_ptr    decl_type    ;
        expr_ptr         default_value;
    };

    struct decl_strg : public decl
    {
        type_id_ptr type ;
        scope_ptr   scope;
    };

    using decl_ptr = std::unique_ptr<decl>;
    using decl_strg_ptr = std::unique_ptr<decl_strg>;
}
