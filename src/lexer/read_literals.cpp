#include <lexer.hpp>
#include <log.hpp>
#include <msg.hpp>
#include <hyc.hpp>
#include <sstream>

namespace hyc::lex
{
    void lexer::read_int_literal(std::stringstream& ss, func_chr_check cnd, char const* literal_name, bool check_empty)
    {
        if(check_empty)
        {
            readnext();
            if(!cnd(m_c)) // unexpected end to the int literal
            {
                spdlog::error(msg::UNEXPECTED_END, literal_name, m_fn, m_line, m_cpos);
                std::exit(exit::LEX_ERROR);
            }
        }
        while(m_c == '0' || m_c == '_') readnext(); // skip all 0s and _s at the beginning
        if(!cnd(m_c)) // we only had 0s
            ss << '0';
        else
            while(cnd(m_c))
            {
                if(m_c != '_')
                    ss << m_c;
                readnext();
            }
    }
    char lexer::read_chr()
    {
        char c;
        if(m_c == '\\') // escape character
        {
            spdlog::debug("Escaped character");
            readnext();
            c = get_escaped(m_c);
            if(c == -1)
            {
                spdlog::error(msg::INVALID_ESCAPE_SEQ, std::string(1, '\\') + m_c, m_fn, m_line, m_cpos);
                std::exit(exit::LEX_ERROR);
            }
        }
        else
        {
            spdlog::debug("Non escaped character");
            c = m_c;
        }
        readnext();
        spdlog::debug("READ CHR will return `{}` while m_c is set to `{}`", c, m_c);
        return c;
    }
}
