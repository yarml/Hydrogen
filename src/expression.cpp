#include <cstdlib>
#include <exit_reason.hpp>
#include <expression.hpp>

namespace  hydrogen::expression
{
    binops str_to_binop(std::string const& str)
    {
        if(str == "="  ) return binops::ASSIGN;
        if(str == "+=" ) return binops::ASSIGN_ADD;
        if(str == "-=" ) return binops::ASSIGN_SUB;
        if(str == "*=" ) return binops::ASSIGN_MUL;
        if(str == "/=" ) return binops::ASSIGN_DIV;
        if(str == "%=" ) return binops::ASSIGN_MOD;
        if(str == "<<=") return binops::ASSIGN_SHIFTL;
        if(str == ">>=") return binops::ASSIGN_SHIFTR;
        if(str == "&=" ) return binops::ASSIGN_AND;
        if(str == "|=" ) return binops::ASSIGN_OR;
        if(str == "^=" ) return binops::ASSIGN_XOR;
        if(str == "~=" ) return binops::ASSIGN_NOT;
        if(str == "==" ) return binops::EQUAL;
        if(str == "!=" ) return binops::NOT_EQUAL;
        if(str == ">"  ) return binops::GREATER;
        if(str == "<"  ) return binops::LESSER;
        if(str == ">=" ) return binops::GREATER_EQ;
        if(str == "<=" ) return binops::LESSER_EQ;
        if(str == "<<" ) return binops::SHIFTL;
        if(str == ">>" ) return binops::SHIFTR;
        if(str == "and") return binops::AND;
        if(str == "or" ) return binops::OR;
        if(str == "xor") return binops::XOR;
        if(str == "&"  ) return binops::BIT_AND;
        if(str == "|"  ) return binops::BIT_OR;
        if(str == "^"  ) return binops::BIT_XOR;
        if(str == "+"  ) return binops::ADD;
        if(str == "-"  ) return binops::SUB;
        if(str == "*"  ) return binops::MUL;
        if(str == "/"  ) return binops::DIV;
        if(str == "%"  ) return binops::MOD;
        std::exit(exit::INVALID_STATE);
    }
    uniops str_to_uniop(std::string const& str)
    {
        if(str == "not") return uniops::NOT    ;
        if(str == "~"  ) return uniops::BIT_NOT;
        if(str == "+"  ) return uniops::ADD    ;
        if(str == "-"  ) return uniops::SUB    ;
        if(str == "++" ) return uniops::INCR   ;
        if(str == "--" ) return uniops::DECR   ;
        std::exit(exit::INVALID_STATE);
    }
}
