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
                out.push_back(token(id.str(), token::type::ID, line, cpos));
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
                    spdlog::error(msg::UNEXPECTED_TOK, m_c, m_fn, m_line, m_cpos);
                    std::exit(exit::LEX_ERROR);
                }
                out.push_back(token(num.str(), token::type::NUM, line, cpos));
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
                    spdlog::error(msg::UNEXPECTED_TOK, m_c, m_fn, m_line, m_cpos);
                    spdlog::info(msg::EXPECTED, msg::CHR_LITERAL_END);
                    std::exit(exit::LEX_ERROR);
                }
                m_chrmode = false;
                readnext(); // get ready for next character
                out.push_back(token(std::string(1, c), token::type::CHR, line, cpos));
            }
            else if(m_c == '"') // read string literal
            {
                m_strmode = true;
                std::stringstream ss;
                int line = m_line, cpos = m_cpos;
                readnext();
                while(m_c != '"')
                    ss << read_chr();
                m_strmode = false;
                readnext();
                out.push_back(token(ss.str(), token::type::STR, line, cpos));
            }
            else if(m_c == '\n')
            {
                out.push_back(token(std::string(1, '\n'), token::type::SEP, m_line - 1, m_cpos));
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
                        out.push_back(token("<DEDENT>", token::type::DEDENT, m_line, 1));
                else if(new_indent > m_indent)
                {
                    if(new_indent - m_indent != 1)
                    {
                        spdlog::error(msg::OVER_INDENT, m_fn, m_line, m_indent, new_indent);
                        std::exit(exit::LEX_ERROR);
                    }
                    out.push_back(token("<INDENT>", token::type::INDENT, m_line, 1));
                }
                m_indent = new_indent;
            }
#include "lex.branch_node.part.hpp"
            SINGLE_BRANCH_NODE(':', ':', COLON, NS_SEP)
            SINGLE_BRANCH_NODE('=', '=', ASSIGN, EQUAL)
            SINGLE_BRANCH_NODE('!', '=', BIT_NOT, NOT_EQUAL)
            SINGLE_BRANCH_NODE('&', '=', BIT_AND, ASSIGN_AND)
            SINGLE_BRANCH_NODE('|', '=', BIT_OR, ASSIGN_OR)
            SINGLE_BRANCH_NODE('^', '=', BIT_XOR, ASSIGN_XOR)
            
            SINGLE_BRANCH_NODE('*', '=', MUL, ASSIGN_MUL)
            SINGLE_BRANCH_NODE('/', '=', DIV, ASSIGN_DIV)
            SINGLE_BRANCH_NODE('%', '=', MOD, ASSIGN_MOD)

            EQ_SINGLE_BRANCH('+', '+', '=', ASSIGN_ADD, ADD, INCR)
            EQ_SINGLE_BRANCH('-', '-', '=', ASSIGN_SUB, SUB, DECR)

            CPLX_BRANCH_NODE('<', '<', '=', '=', ASSIGN_SHIFTL, SHIFTL, LESS_EQ, LESS)
            CPLX_BRANCH_NODE('>', '>', '=', '=', ASSIGN_SHIFTR, SHIFTR, GREAT_EQ, GREATER)
#undef SINGLE_BRANCH_NODE
#undef SMTH_BRANCH_NODE
            else if(m_c == ' ')
                readnext();
            else // tokens that always only consume 1 character will be treated here
            {
                token::type toktype;
                switch(m_c)
                {
                    case '(': toktype = token::type::PARENL; break;
                    case ')': toktype = token::type::PARENR; break;
                    case '{': toktype = token::type::BRACEL; break;
                    case '}': toktype = token::type::BRACER; break;
                    case '[': toktype = token::type::BRACKL; break;
                    case ']': toktype = token::type::BRACKR; break;
                    case ';': toktype = token::type::SEP   ; break;
                    case ',': toktype = token::type::COMMA ; break;
                    default: // otherwise we came across an invalid token
                        spdlog::error(msg::UNEXPECTED_TOK, m_c, m_fn, m_line, m_cpos);
                        std::exit(exit::LEX_ERROR);
                        break;
                }
                out.push_back(token(std::string(1, m_c), toktype, m_line, m_cpos));
                readnext();
            }
        }
        out.push_back(token("<EOF>", token::type::TOK_EOF, m_line, m_cpos));
        spdlog::debug("Finish lex");
        return out;
    }
}
