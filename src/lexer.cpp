#include <lexer.hpp>
#include <spdlog/spdlog.h>
#include <hyc.hpp>
#include <msg.hpp>

namespace hyc::lex
{
    lexer::lexer(std::istream& input)
        : m_input(input),
          m_line(1),
          m_cpos(0),
          m_openp(0), m_openbk(0), m_openbc(0),
          m_c(0),
          m_rc(0),
          m_pc(input.peek()),
          m_strmode(false), m_chrmode(false), m_cmtmode(false)
    { }

    std::vector<int> lexer::lex()
    {
        spdlog::debug("Testing lexer...");
        while(m_pc != LEX_EOF)
        {
            readnext();
            std::cout << m_c;
        }
        spdlog::debug("Test done! hope no sigsegv out  of nowhere or sumthing you never know");
        return {};
    }
    // Returns:
    // if m_c is a space, then the number of spaces that were skipped with it, tabs are counted as 4 spaces
    // returns -1 if not m_c is not a space
    // Side effects:
    // apart from the return value, this function also modifies m_c, which is its primary function
    int lexer::readnext()
    {
        if(m_strmode || m_chrmode) // if in string or char, then everything is valid except EOF
        {
            if(m_pc == LEX_EOF)
            {
                spdlog::error(msg::UNEXPECTED_EOF, m_line, m_cpos, msg::STR_CHR_LITERAL);
                std::exit(hyc::exit::LEX_ERROR);
            }
            readstream();
            m_c = m_rc;
            return -1;
        }
        if(m_pc == ' ' || m_pc == '\t' || (m_pc == '\n' && open_pbck() > 0))
        {
            m_c = ' ';
            int sc = 0; // space count
            while(m_pc == ' ' || m_pc == '\t' || (m_pc == '\n' && open_pbck() > 0))
            {
                readstream();
                switch(m_rc)
                {
                    case ' ' : ++sc     ; break;
                    case '\t':   sc += 4; break;
                    default  :            break;
                }
                if(m_pc == '#')
                {
                    m_cmtmode = true;
                    readstream();
                    if(m_pc == '#') // multiline comment, we still continue skipping spaces after it
                    {
                        spdlog::debug("Skipping multiline comment while skipping spaces");
                        readstream();
                        while(m_pc != '#' || m_rc != '#')
                        {
                            readstream();
                            if(m_pc == LEX_EOF)
                            {
                                spdlog::error(msg::UNEXPECTED_EOF, m_line, m_cpos, msg::MULTILINE_COMMENT);
                                std::exit(exit::LEX_ERROR);
                            }
                        }
                        spdlog::debug("finished from multiline comment while skipping spaces {}:{}", m_rc, m_pc);
                        readstream();
                        m_cmtmode = false;
                    }
                    else // single line comment, we skip it then return, no more reading spaces
                    {
                        while(m_pc != '\n' && m_pc != LEX_EOF)
                            readstream();
                        m_cmtmode = false;
                        // the next character to be read now is \n or EOF, 
                        // cause that's what a single line comment has to end with
                        return sc;
                    }
                }
            }
            return sc;
        }
        if(m_pc == '#')
        {
            m_cmtmode = true;
            readstream();
            if(m_pc == '#')
            {
                spdlog::debug("Skipping multiline comment");
                readstream();
                spdlog::debug("Now that the stream is read the state is {}:{}", m_rc, m_pc);
                while(m_pc != '#' || m_rc != '#')
                {
                    readstream();
                    if(m_pc == LEX_EOF)
                    {
                        spdlog::error(msg::UNEXPECTED_EOF, m_line, m_cpos, msg::MULTILINE_COMMENT);
                        std::exit(exit::LEX_ERROR);
                    }
                }
                spdlog::debug("finished from multiline comment {}:{}", m_rc, m_pc);
                readstream();
                m_cmtmode = false;
            }
            else
            {
                while(m_pc != '\n' && m_rc != LEX_EOF)
                    readstream();
                m_cmtmode = false;
            }
            return readnext();
        }
        // not a special case
        readstream();
        m_c = m_rc;
        return -1;
    }
    void lexer::readstream()
    {
        m_rc = m_input.get();
        m_pc = m_input.peek();
        if(m_rc == '\n')
        {
            ++m_line;
            m_cpos = 0;
        }
        else
            ++m_cpos;
        if(!m_strmode && !m_chrmode && !m_cmtmode)
        {
            switch(m_rc)
            {
                case '(': ++m_openp ; spdlog::debug("open ("); break;
                case '[': ++m_openbk; break;
                case '{': ++m_openbc; break;
                case ')': --m_openp ; spdlog::debug("close )");break;
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
    }

    int lexer::open_pbck()
    {
        if(m_openp < 0 || m_openbk < 0 || m_openbc < 0)
            return -1;
        return m_openp + m_openbk + m_openbc;
    }
}
