#ifndef HYC_STR_HPP
#define HYC_STR_HPP

#include <functional>
#include <cctype>

namespace hyc
{
    using func_chr_check = std::function<bool(char)>;

    inline bool is_dec(char c)
    {
        return std::isdigit(c) || c == '_';
    }
    inline bool is_hex(char c)
    {
        return std::isxdigit(c) || c == '_';
    }
    inline bool is_oct(char c)
    {
        return (std::isdigit(c) && c < '8') || c == '_';
    }
    inline bool is_bin(char c)
    {
        return c == '0' || c == '1' || c == '_';
    }
    inline char get_escaped(char unescaped)
    {
        switch(unescaped)
        {
            case '\'': return '\'';
            case '"' : return '"' ;
            case '?' : return '?' ;
            case '\\': return '\\';
            case 'a' : return '\a';
            case 'b' : return '\b';
            case 'f' : return '\f';
            case 'n' : return '\n';
            case 'r' : return '\r';
            case 't' : return '\t';
            case 'v' : return '\v';
            default  : return -1  ;
        }
    }
}

#endif
