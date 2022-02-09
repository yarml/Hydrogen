#include <lexer.hpp>
#include <log.hpp>

namespace hyc::lex
{
    // Function:
    //   Sets m_c to the next character in the not-to-skip stream
    //   Sets m_sc to -1 unless m_c was set to ' ', then it is set to the number of skipped spaces + skipped tabs * 4
    void lexer::readnext()
    {
        if(m_strmode || m_chrmode) // if in string or char, then everything is valid except EOF
        {
            readstream();
            spdlog::debug("chr_strmode reading stream returns {}", m_rc);
            m_c = m_rc;
            m_sc = -1;
            return;
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
                    default  :            ; break;
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
}
