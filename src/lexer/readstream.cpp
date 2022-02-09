#include <lexer.hpp>
#include <log.hpp>
#include <msg.hpp>
#include <hyc.hpp>

namespace hyc::lex
{
    void lexer::readstream()
    {
        m_rc = m_input.get();
        m_pc = m_input.peek();
        if(m_rc == '\n')
        {
            ++m_line;
            m_cpos = 1;
        }
        else
            ++m_cpos;
        if(!m_strmode && !m_chrmode && !m_cmtmode)
        {
            if(
                (m_openps.size() == '0' && m_rc == ')') 
             || (m_openbks.size() == '0' && m_rc == ']') 
             || (m_openbcs.size() == '0' && m_rc == ']')
            )
            {
                spdlog::error(msg::UNMATCHED_CLOSING_PBKC, m_rc, m_fn, m_line, m_cpos);
                std::exit(hyc::exit::LEX_ERROR);
            }
            switch(m_rc)
            {
                case '(': m_openps.push_back({m_line, m_cpos}) ; break;
                case '[': m_openbks.push_back({m_line, m_cpos}); break;
                case '{': m_openbcs.push_back({m_line, m_cpos}); break;
                case ')': m_openps.pop_back()                  ; break;
                case ']': m_openbks.pop_back()                 ; break;
                case '}': m_openbcs.pop_back()                 ; break;
                default: break;
            }
        }
        if(m_pc == LEX_EOF) // Error when EOF is unexpected
        {
            if(m_mlcmt) // Multiline comment
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_fn, m_line, m_cpos, msg::MULTILINE_COMMENT);
                std::exit(hyc::exit::LEX_ERROR);
            }
            if(m_strmode || m_chrmode) // string/char literal
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_fn, m_line, m_cpos, msg::STR_CHR_LITERAL);
                std::exit(hyc::exit::LEX_ERROR);
            }
            if(m_openps.size() > 0) // unmatched parentheses
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_fn, m_line, m_cpos, msg::PARENS);
                spdlog::info(msg::LAST_OPEN_PBKC, '(', m_fn, m_openps.back().first, m_openps.back().second);
                std::exit(hyc::exit::LEX_ERROR);
            }
            if(m_openbks.size() > 0) // unmatched brackets
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_fn, m_line, m_cpos, msg::BRACKETS);
                spdlog::info(msg::LAST_OPEN_PBKC, '[', m_fn, m_openbks.back().first, m_openbks.back().second);
                std::exit(hyc::exit::LEX_ERROR);
            }
            if(m_openbcs.size() > 0) // unmatched braces
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_fn, m_line, m_cpos, msg::BRACES);
                spdlog::info(msg::LAST_OPEN_PBKC, '{', m_fn, m_openbcs.back().first, m_openbcs.back().second);
                std::exit(hyc::exit::LEX_ERROR);
            }
        }

    }
}
