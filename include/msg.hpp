#ifndef HYC_MSG_HPP
#define HYC_MSG_HPP

#include <string_view>

namespace hyc::msg
{
    constexpr char const* UNEXPECTED_EOF         = "Unexpected EOF at {0}:{1}: cannot end file before ending {2}"                           ;
    constexpr char const* STR_CHR_LITERAL        = "string/char literal"                                                                    ;
    constexpr char const* MULTILINE_COMMENT      = "multi line comment"                                                                     ;
    constexpr char const* PARENS                 = "parentheses ()"                                                                         ;
    constexpr char const* BRACES                 = "braces {}"                                                                              ;
    constexpr char const* BRACKETS               = "brackets []"                                                                            ;
    constexpr char const* UNMATCHED_CLOSING_PBKC = "Unmatched '{0}' at {1}:{2}"                                                             ;
    constexpr char const* INVALID_INDENT         = "Invalid indentation in line {0}: indentation is off by {1} whitespaces"                 ;
    constexpr char const* OVER_INDENT            = "Too much indentation in line {0}: Previous line had {1} indents, while this one has {2}";
    constexpr char const* UNEXPECTED_END         = "Unexpected end of {0} at {1}:{2}"                                                       ;
    constexpr char const* DEC_LITERAL            = "decimal literal"                                                                        ;
    constexpr char const* BIN_LITERAL            = "binary literal"                                                                         ;
    constexpr char const* OCT_LITERAL            = "octal literal"                                                                          ;
    constexpr char const* HEX_LITERAL            = "hexadecimal literal"                                                                    ;
    constexpr char const* UNEXPECTED_CHR         = "Unexpected {0} at {1}:{2}"                                                              ;
}

#endif