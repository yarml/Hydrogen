#include <lexer.hpp>
#include <cstring>

namespace hyc::lex
{
    token::type lexer::get_identifier_type(std::string const& id)
    {
        // using strcmp cause I think it's faster
        char const* str = id.c_str();
#define TEST_STR(s, tok) if(!std::strcmp(str, s)) return token::type::tok;
        // Debug
        TEST_STR("print", PRINT)
        // Declarations
        TEST_STR("def", DEF)
        TEST_STR("decl", DECL)
        TEST_STR("public", PUBLIC)
        TEST_STR("protected", PROTECTED)
        TEST_STR("private", PRIVATE)
        TEST_STR("global", GLOBAL)
        TEST_STR("local", LOCAL)
        TEST_STR("var", VAR)
        TEST_STR("const", CONST)
        TEST_STR("func", FUNC)
        TEST_STR("class", CLASS)
        TEST_STR("returns", RETURNS)
        TEST_STR("access", ACCESS)
        TEST_STR("namespace", NAMESPACE)
        // Statements
        TEST_STR("break", BREAK)
        TEST_STR("continue", CONTINUE)
        TEST_STR("return", RETURN)
        TEST_STR("if", IF)
        TEST_STR("elif", ELIF)
        TEST_STR("else", ELSE)
        TEST_STR("for", FOR)
        TEST_STR("in", IN)
        TEST_STR("foreach", FOREACH)
        TEST_STR("while", WHILE)
        TEST_STR("do", DO)
        TEST_STR("import", IMPORT)
        TEST_STR("from", FROM)
        // literals
        TEST_STR("true", BOOL)
        TEST_STR("false", BOOL)
#undef TEST_STR
        return token::type::ID;
    }
}
