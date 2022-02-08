#ifndef HYC_MSG_HPP
#define HYC_MSG_HPP

#include <string_view>

namespace hyc::msg
{
    using     cstr = char const*;
    constexpr cstr ARG_PARSE_ERROR        = "Error parsing arguments: {0}"                                                          ;
    constexpr cstr FILE_OPEN_ERROR        = "Couldn't open file: {0}"                                                               ;
    constexpr cstr UNEXPECTED_EOF         = "Unexpected EOF at {0}:{1}:{2}: cannot end file before ending {3}"                      ;
    constexpr cstr STR_CHR_LITERAL        = "string/char literal"                                                                   ;
    constexpr cstr MULTILINE_COMMENT      = "multi line comment"                                                                    ;
    constexpr cstr PARENS                 = "parentheses ()"                                                                        ;
    constexpr cstr BRACES                 = "braces {}"                                                                             ;
    constexpr cstr BRACKETS               = "brackets []"                                                                           ;
    constexpr cstr UNMATCHED_CLOSING_PBKC = "Unmatched `{0}` in {1}:{2}:{3}"                                                        ;
    constexpr cstr INVALID_INDENT         = "Invalid indentation in {0}:{1}: indentation is off by {2} whitespaces"                 ;
    constexpr cstr OVER_INDENT            = "Too much indentation in {0}:{1}: Previous line had {2} indents, while this one has {3}";
    constexpr cstr UNEXPECTED_END         = "Unexpected end of {0} in {1}:{2}:{3}"                                                  ;
    constexpr cstr DEC_LITERAL            = "decimal literal"                                                                       ;
    constexpr cstr BIN_LITERAL            = "binary literal"                                                                        ;
    constexpr cstr OCT_LITERAL            = "octal literal"                                                                         ;
    constexpr cstr HEX_LITERAL            = "hexadecimal literal"                                                                   ;
    constexpr cstr UNEXPECTED_CHR         = "Unexpected `{0}` at {1}:{2}:{3}"                                                       ;
    constexpr cstr LAST_OPEN_PBKC         = "Unmatched `{0}` is in {1}:{2}:{3}"                                                     ;
    constexpr cstr EXPECTED               = "Expected {0}"                                                                          ;
    constexpr cstr CHR_LITERAL_END        = "`'` to end char literal"                                                             ;
    constexpr cstr INVALID_ESCAPE_SEQ     = "Invalid escape sequence `{0}` in {1}:{2}:{3}"                                          ;
    constexpr cstr EMPTY_CHR_LITERAL      = "Unexpected end to char literal in {0}:{1}:{2}: char literal can't be empty"            ;
}

#endif