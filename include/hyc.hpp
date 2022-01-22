#pragma once

#include <string_view>
#include <iostream>
#include <log.hpp>

namespace hyc
{
    constexpr bool DEBUG = false;
    constexpr std::string_view NAME = "Hydrogen Compiler";
    constexpr std::string_view ID   = "hyc"              ;
    constexpr std::string_view VER  = "0.0.1"            ;

    void start(
        std::string const& file_name         ,
        std::istream&      input_stream      ,
        std::ostream&      output_stream     ,
        int                optimisation_level,
        logger::level      log_level         ,
        bool               debug_mode
    );
}
