#include <ast.hpp>

namespace hydrogen::ast
{
    // node
    node_t::node_t()
    {
        m_node_type = node_type::NONE;
    }
    node_t::~node_t() { }
    node_type node_t::get_node_type() const
    {
        return m_node_type;
    }
    // expr
    expr_t::expr_t()
    {
        m_node_type = node_type::EXPR;
    }
    // stmt
    stmt_t::stmt_t()
    {
        m_node_type = node_type::STMT;
    }
    // num_expr
    num_expr_t::num_expr_t(int value)
    {
        m_node_type = node_type::EXPR_NUM;
        m_value = value;
    }
    int num_expr_t::get_value() const
    {
        return m_value;
    }

    // str_expr
    str_expr_t::str_expr_t(std::string const& value)
    {
        m_node_type = node_type::EXPR_STR;
        m_value = value;
    }
    std::string const& str_expr_t::get_value() const
    {
        return m_value;
    }
    // id_expr
    id_expr_t::id_expr_t(std::string const& id)
    {
        m_node_type = node_type::EXPR_ID;
        m_id = id;
    }
    std::string const& id_expr_t::get_id() const
    {
        return m_id;
    }
    // binop_expr
    binop_expr_t::binop_expr_t(expression::binops op, expr const& left, expr const& right)
    {
        m_node_type = node_type::EXPR_BINOP;
        m_op    = op   ;
        m_left  = left ;
        m_right = right;
    }
    expression::binops binop_expr_t::get_op() const
    {
        return m_op;
    }
    expr const& binop_expr_t::get_left_expr() const
    {
        return m_left;
    }
    expr const& binop_expr_t::get_right_expr() const
    {
        return m_right;
    }
    // uniop_expr
    uniop_expr_t::uniop_expr_t(expression::uniops op, expr const& expr)
    {
        m_node_type = node_type::EXPR_UNIOP;
        m_op_before = true;
        m_expr = expr;
    }
    uniop_expr_t::uniop_expr_t(expr const& expr, expression::uniops op)
    {
        m_node_type = node_type::EXPR_UNIOP;
        m_op_before = false;
        m_expr = expr;
    }

    bool uniop_expr_t::is_op_before() const
    {
        return m_op_before;
    }
    expression::uniops uniop_expr_t::get_op() const
    {
        return m_op;
    }
    expr const&uniop_expr_t::get_expr() const
    {
        return m_expr;
    }
    // dhdef_stmt
    dhdef_stmt_t::dhdef_stmt_t(data_holder::scope scope, data_holder::type_spec type, std::string const& name, expr const& expr)
    {
        m_node_type = node_type::STMT_DHDEF;
        m_scope = scope;
        m_type = type;
        m_name = name;
        m_expr = expr;
    }
    
    data_holder::scope dhdef_stmt_t::get_scope() const
    {
        return m_scope;
    }
    data_holder::type_spec dhdef_stmt_t::get_type() const
    {
        return m_type;
    }
    std::string const& dhdef_stmt_t::get_name() const
    {
        return m_name;
    }
    expr const& dhdef_stmt_t::get_expr() const
    {
        return m_expr;
    }
    // print_stmt
    print_stmt_t::print_stmt_t(expr const& expr)
    {
        m_node_type = node_type::STMT_PRINT;
        m_expr = expr;
    }
    expr const& print_stmt_t::get_expr() const
    {
        return m_expr;
    }

    // source_t
    source_t::source_t(std::vector<stmt> const& statements)
    {
        m_node_type = node_type::SOURCE;
        m_statements = statements;
    }
    std::vector<stmt> const& source_t::get_statements() const
    {
        return m_statements;
    }
    std::vector<stmt>& source_t::get_statements()
    {
        return m_statements;
    }
}
