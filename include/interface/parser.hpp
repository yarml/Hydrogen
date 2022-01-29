#pragma once

#include <interface/lexer.hpp>
#include <ast/source.hpp>

namespace hyc
{
    using tree     = ast::source          ;
    using tree_ptr = std::unique_ptr<tree>;

    tree_ptr parse(token_stream_ptr& ts);
}
