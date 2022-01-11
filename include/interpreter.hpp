#pragma once

#include <iostream>
#include <string>
#include <map>
#include <exit_reason.hpp>
#include <utils.hpp>
#include <ast.hpp>

namespace hydrogen
{
    // The following is just a test interpreter for debug purposes
    template <int> // Just having this here to deal away with defining the class members here
    class interpreter
    {
    public:
        interpreter() = default;
    public:
        void interpret(ast::source src)
        {
            for(ast::stmt const& s : src->get_statements())
                execute_stmt(s);
        }
    private:
        void execute_stmt(ast::stmt const& stmt)
        {
            switch(stmt->get_node_type())
            {
                case ast::node_type::STMT_DHDEF:
                {
                    ast::dhdef_stmt dhdef_stmt = std::dynamic_pointer_cast<ast::dhdef_stmt_t>(stmt);
                    m_variables[dhdef_stmt->get_name()] = get_expr_val(dhdef_stmt->get_expr());
                    break;
                }
                case ast::node_type::STMT_PRINT:
                {
                    std::cout << get_expr_val(std::dynamic_pointer_cast<ast::print_stmt_t>(stmt)->get_expr());
                    break;
                }
                case ast::node_type::STMT: // empty statements
                    break;
                default:
                {
                    std::cout << "Unknown statement type: " << (int) stmt->get_node_type() << "\n";
                    std::exit(exit::INVALID_STATE);
                    break;
                }
            }
        }
        int get_expr_val(ast::expr e)
        {
            int val;
            switch(e->get_node_type())
            {
                case ast::node_type::EXPR_NUM:
                {
                    val = std::dynamic_pointer_cast<ast::num_expr_t>(e)->get_value();
                    break;
                }
                case ast::node_type::EXPR_STR:
                    return 0;
                case ast::node_type::EXPR_ID:
                    return m_variables[std::dynamic_pointer_cast<ast::id_expr_t>(e)->get_id()];
                default:
                    std::cout << "Unknown expression type: " << (int) e->get_node_type() << "\n";
                    std::exit(exit::INVALID_STATE);
                    break;
            }
            return val;
        }
    private:
        std::map<std::string, int> m_variables;
    };
}
