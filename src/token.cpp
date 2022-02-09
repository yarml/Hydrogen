#include <token.hpp>

namespace hyc::lex
{
    token::token(
        std::string const& text,
        type               type,
        int                line,
        int                cpos)
        : m_text(text),
          m_type(type),
          m_line(line),
          m_cpos(cpos)
    { }

    std::string const& token::get_text() const { return m_text; }
    std::string      & token::get_text()       { return m_text; }
    token::type        token::get_type()       { return m_type; }
    token::type        token::get_type() const { return m_type; }
    int                token::get_line() const { return m_line; }
    int                token::get_cpos() const { return m_cpos; }

    void token::set_text(std::string const& text) { m_text = text; }
    void token::set_type(type               type) { m_type = type; }
    void token::set_line(int                line) { m_line = line; }
    void token::set_cpos(int                cpos) { m_cpos = cpos; }
}
