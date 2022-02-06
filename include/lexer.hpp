#ifndef HYC_LEX_HPP
#define HYC_LEX_HPP

#include <istream>
#include <string>
#include <vector>

namespace hyc::lex
{
    class lexer
    {
    public:
        lexer(std::istream& input);
    public:
        std::vector<int> lex();
    private:
        void readstream();
        int readnext();

        // returns the number of open ()[]{} combined, or -1 if any of them is invalid
        int open_pbck();
    private:
        std::istream& m_input;
        int m_line, m_cpos;
        int m_openp, m_openbk, m_openbc;
        char m_c;
        char m_rc;
        char m_pc;
        bool m_strmode, m_chrmode, m_cmtmode;
    };

    constexpr int LEX_EOF = std::istream::traits_type::eof();
}

#endif
