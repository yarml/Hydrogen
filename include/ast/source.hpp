#pragma once

#include <vector>
#include <memory>
#include <ast/decl.hpp>

namespace hyc::ast
{
    struct source
    {
        std::vector<decl_ptr> decls;
    };
}
