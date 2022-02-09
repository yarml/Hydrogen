#ifndef HYC_HPP
#define HYC_HPP

#include <string_view>
#include <string>
#include <iostream>

namespace hyc
{
    constexpr bool             DEBUG = false              ;
    constexpr std::string_view NAME  = "Hydrogen compiler";
    constexpr std::string_view ID    = "hyc"              ;
    constexpr std::string_view VER   = "0.0.1"            ;

    void start(
        std::string const& file_name         ,
        std::istream&      input_stream      ,
        std::ostream&      output_stream     ,
        int                optimisation_level,
        bool               debug_mode
    );

    namespace exit
    {
        using exit_code = int;

        constexpr exit_code SUCCESS      = 0x00;
        constexpr exit_code INVALID_ARGS = 0x10;
        constexpr exit_code FILE_ERROR   = 0x20;
        constexpr exit_code LEX_ERROR    = 0x30;
        constexpr exit_code PARSE_ERROR  = 0x40;
    }
}

#endif
