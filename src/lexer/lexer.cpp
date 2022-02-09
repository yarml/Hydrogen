#include <lexer.hpp>

namespace hyc::lex
{
    lexer::lexer(std::istream& input, std::string const& fn)
        : m_fn(fn),
          m_input(input),
          m_line(1),
          m_cpos(0),
          m_sc(0),
          m_indent(0),
          m_c(0),
          m_rc(0),
          m_pc(input.peek()),
          m_strmode(false), m_chrmode(false), m_cmtmode(false), m_mlcmt(false)
    { }   
}
