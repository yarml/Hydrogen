#ifndef HYC_LEX_HPP
#define HYC_LEX_HPP

#include <token.hpp>
#include <str.hpp>
#include <istream>
#include <string>
#include <vector>
#include <utility>

namespace hyc::lex
{
    class lexer
    {
    public:
        lexer(std::istream& input, std::string const& filename);
    public:
        std::vector<token> lex();
    private:
        void readnext();
        void readstream();

        // returns the number of open ()[]{} combined, or -1 if any of them is invalid
        int open_pbck();

        // reads the next set of characters c for which condition(c) is true, then pushes them to ss
        void read_int_literal(std::stringstream& ss, func_chr_check condition, char const* literal_name, bool check_empty = true);
        char read_chr();
    private:
        std::vector<std::pair<int, int>> m_openps ; // open parentheses
        std::vector<std::pair<int, int>> m_openbks; // open brackets
        std::vector<std::pair<int, int>> m_openbcs; // open braces
        std::string                      m_fn     ; // file name(used for error report)
        std::istream&                    m_input  ; // input stream
        int                              m_line   ; // current line
        int                              m_cpos   ; // character position
    private: // this is the internal state of the lexer, forgive the variable names
        int                              m_sc     ; // space count(updated on readnext call)
        int                              m_indent ; // stores the current number of indents
        char                             m_c      ; // current character(filtered by readnext)
        char                             m_rc     ; // current character(real, set by readstream)
        char                             m_pc     ; // next character in input(input.peek)
        bool                             m_strmode; // string literal
        bool                             m_chrmode; // char literal
        bool                             m_cmtmode; // comment
        bool                             m_mlcmt  ; // multiline comment
    };

    constexpr int LEX_EOF = std::istream::traits_type::eof();
}

#endif
