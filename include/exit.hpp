#pragma once

#include <string>

namespace hyc::exit
{
    using exit_code = int;
    
    constexpr exit_code SUCCESS      = 0x00;
    constexpr exit_code INVALID_ARGS = 0x10;
    constexpr exit_code FILE_ERROR   = 0x20;
    constexpr exit_code PARSE_EROOR  = 0x30;

    [[noreturn]]
    void exit(exit_code ec, std::string const& exit_msg, std::string const& verbose_msg="");
}
