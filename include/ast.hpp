#pragma once

#include <variant>
#include <memory>
#include <vector>
#include <string>
#include <data_holder.hpp>
#include <expression.hpp>

namespace hydrogen::ast
{
    enum class node_type
    {
        NONE      ,

        SOURCE    ,

        STMT      ,
        STMT_DHDEF,
        STMT_PRINT,
        
        EXPR      ,
        EXPR_NUM  ,
        EXPR_STR  ,
        EXPR_ID   ,
        EXPR_BINOP,
        EXPR_UNIOP
    };

    class node_t      ;
    class expr_t      ;
    class stmt_t      ;
    class num_expr_t  ;
    class str_expr_t  ;
    class id_expr_t   ;
    class binop_expr_t;
    class uniop_expr_t;
    class dhdef_stmt_t;
    class print_stmt_t;
    class source_t    ;

    using node       = std::shared_ptr<node_t      >;
    using expr       = std::shared_ptr<expr_t      >;
    using stmt       = std::shared_ptr<stmt_t      >;
    using num_expr   = std::shared_ptr<num_expr_t  >;
    using str_expr   = std::shared_ptr<str_expr_t  >;
    using id_expr    = std::shared_ptr<id_expr_t   >;
    using binop_expr = std::shared_ptr<binop_expr_t>;
    using uniop_expr = std::shared_ptr<uniop_expr_t>;
    using dhdef_stmt = std::shared_ptr<dhdef_stmt_t>;
    using print_stmt = std::shared_ptr<print_stmt_t>;
    using source     = std::shared_ptr<source_t    >;

    class node_t
    {
    public:
        node_t();
        virtual ~node_t();
    public:
        node_type get_node_type() const;
    protected:
        node_type m_node_type;
    };
    class expr_t : public node_t
    { 
    public:
        expr_t();
    };
    class stmt_t : public node_t
    { 
    public:
        stmt_t();
    };
    class num_expr_t : public expr_t
    {
    public:
        num_expr_t(int value);
    public:
        int get_value() const;
    private:
        int m_value;
    };
    class str_expr_t : public expr_t
    {
    public:
        str_expr_t(std::string const& value);
    public:
        std::string const& get_value() const;
    private:
        std::string m_value;
    };
    class id_expr_t : public expr_t
    {
    public:
        id_expr_t(std::string const& id);
    public:
        std::string const& get_id() const;
    private:
        std::string m_id;
    };
    class binop_expr_t : public expr_t
    {
    public:
        binop_expr_t(expression::binops op, expr const& left, expr const& right);
    public:
        expression::binops get_op(        ) const;
        expr const&        get_left_expr( ) const;
        expr const&        get_right_expr() const;
    private:
        expression::binops m_op   ;
        expr               m_left ;
        expr               m_right;
    };
    class uniop_expr_t : public expr_t
    {
    public:
        uniop_expr_t(expression::uniops op, expr const& expr);
        uniop_expr_t(expr const& expr, expression::uniops op);
    public:
        bool               is_op_before() const;
        expression::uniops get_op(      ) const;
        expr const&        get_expr(    ) const;
    private:
        bool               m_op_before;
        expression::uniops m_op       ;
        expr               m_expr     ;
    };
    class dhdef_stmt_t : public stmt_t
    {
    public:
        dhdef_stmt_t(data_holder::scope scope, data_holder::type_spec type, std::string const& name, expr const& expr);
    public:
        data_holder::scope     get_scope() const;
        data_holder::type_spec get_type( ) const;
        std::string const&     get_name( ) const;
        expr        const&     get_expr( ) const;
    private:
        data_holder::scope     m_scope;
        data_holder::type_spec m_type ;
        std::string            m_name ;
        expr                   m_expr ;
    };
    class print_stmt_t : public stmt_t
    {
    public:
        print_stmt_t(expr const& expr);
    public:
        expr const& get_expr() const;
    private:
        expr m_expr;
    };
    class source_t : public node_t
    {
    public:
        source_t(std::vector<stmt> const& statements);
    public:
        std::vector<stmt> const& get_statements() const;
        std::vector<stmt>&       get_statements()      ;
    private:
        std::vector<stmt> m_statements;
    };
}
