#ifndef HYC_TOK_HPP
#define HYC_TOK_HPP

#include <string>
#include <vector>

namespace hyc::lex
{
    class token
    {
    public:
        token(
            std::string const& text,
            std::string const& type,
            int                line,
            int                cpos);
    public:
        std::string const& get_text() const;
        std::string      & get_text()      ;
        std::string      & get_type()      ;
        std::string const& get_type() const;
        int                get_line() const;
        int                get_cpos() const;
    public:
        void set_text(std::string const& text);
        void set_type(std::string const& type);
        void set_line(int                line);
        void set_cpos(int                cpos);
    private:
        std::string           m_text;
        std::string           m_type;
        int                   m_line;
        int                   m_cpos;
    };
}

#endif