#pragma once

#include <cstddef>
#include <memory>

namespace hyc::ast
{
    struct node
    {
        std::size_t line;
        std::size_t cpos;
    };

    using node_ptr = std::unique_ptr<node>;
}
