#pragma once

#include <cstddef>
#include <string>

namespace hydrogen::data_holder
{
    enum class scope
    {
        LOCAL, GLOBAL
    };
    enum class container_type
    {
        VAR, CONST
    };
    struct type_spec
    {
        container_type container_type;
        std::size_t size;
    };
}

