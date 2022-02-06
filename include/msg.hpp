#ifndef HYC_MSG_HPP
#define HYC_MSG_HPP

#include <string_view>

namespace hyc::msg
{
    constexpr char const* UNEXPECTED_EOF         = "Unexpected EOF at {0}:{1}: cannot end file before ending {2}";
    constexpr char const* STR_CHR_LITERAL        = "string/char literal"                                         ;
    constexpr char const* MULTILINE_COMMENT      = "multi line comment"                                          ;
    constexpr char const* UNMATCHED_CLOSING_PBKC = "Unmatched '{0}' at {1}:{2}"                                  ;
}

#endif