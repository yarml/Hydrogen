#ifndef HYC_LEX_HPP
#define HYC_LEX_HPP

#include <token.hpp>
#include <str.hpp>
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
        std::vector<token> lex();
    private:
        void readnext();
        void readstream();

        // returns the number of open ()[]{} combined, or -1 if any of them is invalid
        int open_pbck();

        //
        void read_int_literal(std::stringstream& ss, func_chr_check condition, char const* literal_name, bool check_empty = true);
    private:
        std::istream& m_input  ;
        int           m_line   ;
        int           m_cpos   ; // character position
        int           m_openp  ; // open parentheses
        int           m_openbk ; // open brackets
        int           m_openbc ; // open braces
        int           m_sc     ; // space count(updated on readnext call)
        int           m_indent ; // stores the current number of indents
        char          m_c      ; // current character(filtered by readnext)
        char          m_rc     ; // current character(real, set by readstream)
        char          m_pc     ; // next character in input(input.peek)
        bool          m_strmode; // string literal
        bool          m_chrmode; // char literal
        bool          m_cmtmode; // comment
        bool          m_mlcmt  ; // multiline comment
        bool          m_nl     ; // new line
    };

    constexpr int LEX_EOF = std::istream::traits_type::eof();
}

#endif
