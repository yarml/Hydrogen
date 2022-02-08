#include <lexer.hpp>
#include <msg.hpp>
#include <log.hpp>
#include <hyc.hpp>
#include <sstream>

namespace hyc::lex
{
        std::vector<token> lexer::lex()
    {
        spdlog::debug("Lexing");
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
                // if we still have a decimal after we finished reading the current number
                // then we are in a situation like 0b10102, this is an error,
                // but will be considered 'BIN 1010' followed by 'DEC 2' without the code below to handle the error
                // or 0c1359, 9 is invalid octal
                if(is_dec(m_c))
                {
                    spdlog::error(msg::UNEXPECTED_CHR, m_c, m_fn, m_line, m_cpos);
                    std::exit(exit::LEX_ERROR);
                }
                out.push_back(token(num.str(), "NUM", line, cpos));
            }
            else if(m_c == '\'') // char literal
            {
                m_chrmode = true;
                int line = m_line, cpos = m_cpos;
                readnext(); // out cursor now is on the first character
                if(m_c == '\'') // empty char literal -> syntax error
                {
                    spdlog::error(msg::EMPTY_CHR_LITERAL, m_fn, m_line, m_cpos);
                    std::exit(exit::LEX_ERROR);
                }
                char c = read_chr(); // returns character, and sets cursor on the character after, handles escape characters
                if(m_c != '\'') // if we don't have a ' then the char literal was longer than expected -> error
                {
                    spdlog::error(msg::UNEXPECTED_CHR, m_c, m_fn, m_line, m_cpos);
                    spdlog::info(msg::EXPECTED, msg::CHR_LITERAL_END);
                    std::exit(exit::LEX_ERROR);
                }
                m_chrmode = false;
                readnext(); // get ready for next character
                out.push_back(token(std::string(1, c), "CHR", line, cpos));
            }
            else if(m_c == '"') // read string literal
            {
                spdlog::debug("String literal");
                m_strmode = true;
                std::stringstream ss;
                int line = m_line, cpos = m_cpos;
                readnext();
                while(m_c != '"')
                    ss << read_chr();
                m_strmode = false;
                readnext();
                out.push_back(token(ss.str(), "STR", line, cpos));
            }
            else if(m_c == '\n')
            {
                readnext();
                int sc = 0;
                int new_indent = 0;
                if(m_c == ' ')
                {
                    sc = m_sc;
                    new_indent = sc / 4;
                    readnext();
                    if(m_c == '\n')
                    {
                        spdlog::debug("Ignoring indents in line {} because it is empty", m_line);
                        continue;
                    }
                    if(sc % 4 != 0) // invalid indentation
                    {
                        spdlog::error(msg::INVALID_INDENT, m_fn, m_line, sc % 4);
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
                        spdlog::error(msg::OVER_INDENT, m_fn, m_line, m_indent, new_indent);
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
}
