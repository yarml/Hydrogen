#include <lexer.hpp>

namespace hyc::lex
{
    // open parentheses/braces/brackets
    int lexer::open_pbck()
    {
        return m_openps.size() + m_openbks.size() + m_openbcs.size();
    }
}
