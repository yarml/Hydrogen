#include <spdlog/spdlog.h>
#include <lexer.hpp>
#include <sstream>
#include <hyc.hpp>
#include <msg.hpp>
#include <cctype>
#include <str.hpp>

namespace hyc::lex
{
    lexer::lexer(std::istream& input)
        : m_input(input),
          m_line(1),
          m_cpos(0),
          m_openp(0), m_openbk(0), m_openbc(0),
          m_sc(0),
          m_indent(0),
          m_c(0),
          m_rc(0),
          m_pc(input.peek()),
          m_strmode(false), m_chrmode(false), m_cmtmode(false), m_mlcmt(false)
    { }

    std::vector<token> lexer::lex()
    {
        std::vector<token> out;
        readnext();
        while(m_c != LEX_EOF)
        {
            if(std::isalpha(m_c))
            {
                int line = m_line, cpos = m_cpos;
                std::stringstream id;
                while(std::isalnum(m_c) || m_c == '_')
                {
                    id << m_c;
                    readnext();
                }
                out.push_back(token(id.str(), "ID", line, cpos));
                continue;
            }
            else if(std::isdigit(m_c))
            {
                int line = m_line, cpos = m_cpos;
                std::stringstream num;
                if(m_c == '0')
                {
                    readnext();
                    if(m_c == 'x' || m_c == 'X') // hexadecimal mode
                    {
                        num << "HEX ";
                        read_int_literal(num, is_hex, msg::HEX_LITERAL);
                    }
                    else if(m_c == 'b' || m_c == 'B') // binary mode
                    {
                        num << "BIN ";
                        read_int_literal(num, is_bin, msg::BIN_LITERAL);
                    }
                    else if(m_c == 'c' || m_c == 'X') // octal mode
                    {
                        num << "OCT ";
                        read_int_literal(num, is_oct, msg::OCT_LITERAL);
                    }
                    else if(m_c == 'd' || m_c == 'D') // decimal mode
                    {
                        num << "DEC ";
                        read_int_literal(num, is_dec, msg::DEC_LITERAL);
                    }
                    else if(is_dec(m_c)) // decimal without 0d but starts with 0 nontheless
                    {
                        num << "DEC ";
                        read_int_literal(num, is_dec, msg::DEC_LITERAL, false);
                    }
                    else // else, we only had that zero 
                        num << "DEC 0";
                }
                else
                {
                    num << "DEC ";
                    read_int_literal(num, is_dec, msg::DEC_LITERAL, false);
                }
                // if we still have a decimal after we finished read the current number
                // then we are in the situation of 0b10102, this is an error, but will be considered 'BIN 1010' + 'DEC 2'
                // without the code below to handle the error
                // or 0c1359, 9 is invalid octal
                if(is_dec(m_c))
                {
                    spdlog::error(msg::UNEXPECTED_CHR, m_c, m_line, m_cpos);
                    std::exit(exit::LEX_ERROR);
                }
                out.push_back(token(num.str(), "NUM", line, cpos));
            }
            else if(m_c == '\n')
            {
                readnext();
                int sc = 0;
                int new_indent = 0;
                spdlog::debug("New line {} and next is '{}'", m_line, m_c);
                if(m_c == ' ')
                {
                    sc = m_sc;
                    new_indent = sc / 4;
                    spdlog::debug("After new line {}, there is {} space which gives {} indent", m_line, m_sc, new_indent);
                    readnext();
                    if(m_c == '\n')
                    {
                        spdlog::debug("Ignoring indents in line {} because it is empty", m_line);
                        continue;
                    }
                    if(sc % 4 != 0) // invalid indentation
                    {
                        spdlog::error(msg::INVALID_INDENT, m_line, sc % 4);
                        std::exit(exit::LEX_ERROR);
                    }
                }
                if(new_indent < m_indent)
                    for(int i = 0; i < m_indent - new_indent; ++i)
                        out.push_back(token("<DEDENT>", "DEDENT", m_line, 1));
                else if(new_indent > m_indent)
                {
                    if(new_indent - m_indent != 1)
                    {
                        spdlog::error(msg::OVER_INDENT, m_line, m_indent, new_indent);
                        std::exit(exit::LEX_ERROR);
                    }
                    out.push_back(token("<INDENT>", "INDENT", m_line, 1));
                }
                m_indent = new_indent;
            }
            else if(m_c == ' ')
                readnext();
            else
            {
                out.push_back(token(std::string(1, m_c), "UNKNOWN", m_line, m_cpos));
                readnext();
            }
        }
        out.push_back(token("<EOF>", "EOF", m_line, m_cpos));
        spdlog::debug("Finish lex");
        return out;
    }
    // Function:
    //   Sets m_c to the next character in the not-to-skip stream
    //   Sets m_sc to -1 unless m_c was set to ' ', then it is set to the number of skipped spaces + skipped tabs * 4
    void lexer::readnext()
    {
        if(m_strmode || m_chrmode) // if in string or char, then everything is valid except EOF
        {
            readstream();
            m_c = m_rc;
            m_sc = -1;
        }
        if(m_pc == ' ' || m_pc == '\t' || (m_pc == '\n' && open_pbck() > 0))
        {
            m_c = ' ';
            m_sc = 0;
            while(m_pc == ' ' || m_pc == '\t' || (m_pc == '\n' && open_pbck() > 0))
            {
                readstream();
                switch(m_rc)
                {
                    case ' ' : ++m_sc     ; break;
                    case '\t':   m_sc += 4; break;
                    default  :          ; break;
                }
                if(m_pc == '#') // I'm not sure if there is a way to merge this with the same section for skipping normal comments
                {
                    m_cmtmode = true;
                    readstream();
                    if(m_pc == '#') // multiline comment, we still continue skipping spaces after it
                    {
                        m_mlcmt = true;
                        readstream();
                        while(m_pc != '#' || m_rc != '#')
                            readstream();
                        readstream();
                        m_mlcmt = false;
                        m_cmtmode = false;
                    }
                    else // single line comment, we skip it then return, no more reading spaces
                    {
                        while(m_pc != '\n' && m_pc != LEX_EOF)
                            readstream();
                        m_cmtmode = false;
                        // the next character to be read now is \n or EOF, 
                        // cause that's what a single line comment has to end with
                    }
                }
            }
            return;
        }
        if(m_pc == '#')
        {
            m_cmtmode = true;
            readstream();
            if(m_pc == '#') // multiline
            {
                m_mlcmt = true;
                readstream();
                while(m_pc != '#' || m_rc != '#')
                    readstream();
                readstream();
                m_mlcmt = false;
                m_cmtmode = false;
            }
            else // single line
            {
                while(m_pc != '\n' && m_rc != LEX_EOF)
                    readstream();
                m_cmtmode = false;
            }
            readnext();
            return;
        }
        // not a special case
        readstream();
        m_c = m_rc;
        m_sc = -1;
    }
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
            switch(m_rc)
            {
                case '(': ++m_openp ; break;
                case '[': ++m_openbk; break;
                case '{': ++m_openbc; break;
                case ')': --m_openp ; break;
                case ']': --m_openbk; break;
                case '}': --m_openbc; break;
                default: break;
            }
            if(open_pbck() < 0)
            {
                spdlog::error(msg::UNMATCHED_CLOSING_PBKC, m_rc, m_line, m_cpos);
                std::exit(hyc::exit::LEX_ERROR);
            }
        }
        if(m_pc == LEX_EOF) // Error when EOF is unexpected
        {
            if(m_mlcmt) // Multiline comment
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_line, m_cpos, msg::MULTILINE_COMMENT);
                std::exit(hyc::exit::LEX_ERROR);
            }
            if(m_strmode || m_chrmode) // string/char literal
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_line, m_cpos, msg::STR_CHR_LITERAL);
                std::exit(hyc::exit::LEX_ERROR);
            }
            if(m_openp > 0) // unmatched parentheses
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_line, m_cpos, msg::PARENS);
                std::exit(hyc::exit::LEX_ERROR);
            }
            if(m_openbc > 0) // unmatched braces
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_line, m_cpos, msg::BRACES);
                std::exit(hyc::exit::LEX_ERROR);
            }
            if(m_openbk > 0) // unmatched brackets
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_line, m_cpos, msg::BRACKETS);
                std::exit(hyc::exit::LEX_ERROR);
            }
        }

    }

    // open parentheses/braces/brackets
    int lexer::open_pbck()
    {
        if(m_openp < 0 || m_openbk < 0 || m_openbc < 0)
            return -1;
        return m_openp + m_openbk + m_openbc;
    }

    void lexer::read_int_literal(std::stringstream& ss, func_chr_check cnd, char const* literal_name, bool check_empty)
    {
        if(check_empty)
        {
            readnext();
            if(!cnd(m_c)) // unexpected end to the int literal
            {
                spdlog::error(msg::UNEXPECTED_END, literal_name, m_line, m_cpos);
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
}
