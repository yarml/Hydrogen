#include <cstdlib>
#include <hydrogen_visitor.hpp>
#include <exit_reason.hpp>
#include <utils.hpp>
#include <ast.hpp>

namespace hydrogen
{
    ast::source hydrogen_visitor::visit_source(HydrogenParser::SourceContext* ctx) 
    {
        ast::source src = std::make_shared<ast::source_t>(std::vector<ast::stmt>());

        for(HydrogenParser::StmtContext* stmt : ctx->stmt())
            src->get_statements().push_back(visit_stmt(stmt));
        
        return src;
    }
    ast::stmt hydrogen_visitor::visit_stmt(HydrogenParser::StmtContext* stmt) 
    {
        if(stmt->dataholder_def())
            return visit_dataholder_def(stmt->dataholder_def());
        if(stmt->print_stmt())
            return visit_print_stmt(stmt->print_stmt());
        return std::make_shared<ast::stmt_t>(ast::stmt_t());
    }
    ast::print_stmt hydrogen_visitor::visit_print_stmt(HydrogenParser::Print_stmtContext* pr) 
    {
        return std::make_shared<ast::print_stmt_t>(ast::print_stmt_t(visit_expr(pr->expr())));
    }
    ast::dhdef_stmt hydrogen_visitor::visit_dataholder_def(HydrogenParser::Dataholder_defContext* dhd_ctx) 
    {
        ast::expr default_expression;
        if(dhd_ctx->expr())
            default_expression = visit_expr(dhd_ctx->expr());
        else
            default_expression = std::make_shared<ast::expr_t>(ast::num_expr_t(0));
        return std::make_shared<ast::dhdef_stmt_t>
        (
            ast::dhdef_stmt_t(
                visit_scope_spec(dhd_ctx->scope_spec()),
                visit_type_spec(dhd_ctx->type_spec()),
                dhd_ctx->ID()->getText(),
                default_expression
            )
        );
    }
    data_holder::type_spec hydrogen_visitor::visit_type_spec(HydrogenParser::Type_specContext* tspec) 
    {
        std::size_t size;
        std::string type_name = tspec->type_id()->ID()->getText();
        if(type_name == "byte")
            size = 1;
        else if(type_name == "short")
            size = 2;
        else if(type_name == "long")
            size = 8;
        else
            size = 4; // 4 for everything else for now
        // TODO: the size of the type shouldn't be specified in the AST, but by the target module
        return data_holder::type_spec
        {
            .container_type = visit_dataholder_type_spec(tspec->dataholder_type_spec()),
            .size = size
        };
    }
    data_holder::container_type hydrogen_visitor::visit_dataholder_type_spec(
        HydrogenParser::Dataholder_type_specContext* dht_spec
    )
    {
        if(dht_spec)
        {
            if(dht_spec->CONST())
                return data_holder::container_type::CONST;
            if(dht_spec->VAR())
                return data_holder::container_type::VAR;
            std::exit(exit::INVALID_STATE);
        }
        return data_holder::container_type::CONST;
    }
    data_holder::scope hydrogen_visitor::visit_scope_spec(HydrogenParser::Scope_specContext* sspec)
    {
        if(sspec)
        {
            if(sspec->LOCAL())
                return data_holder::scope::LOCAL;
            if(sspec->GLOBAL())
                return data_holder::scope::GLOBAL;
            std::exit(exit::INVALID_STATE);
        }
        return data_holder::scope::LOCAL;
    }
    ast::expr hydrogen_visitor::visit_expr(HydrogenParser::ExprContext* expr) 
    {
        if(expr->NUM())
            return std::make_shared<ast::num_expr_t>(ast::num_expr_t(string_to_int(expr->NUM()->getText())));
        else if(expr->STR())
            return std::make_shared<ast::str_expr_t>(ast::str_expr_t(expr->STR()->getText()));
        else if(expr->ID())
            return std::make_shared<ast::id_expr_t>(ast::id_expr_t(expr->ID()->getText()));
        else if(expr->PARENL() && expr->expr().size() == 1 && expr->PARENR())
            return visit_expr(expr->expr()[0]);
        else if(expr->BINOP() && expr->expr().size() == 2)
            return std::make_shared<ast::binop_expr_t>
            (
                ast::binop_expr_t
                (
                    expression::str_to_binop(expr->BINOP()->getText()),
                    visit_expr(expr->expr()[0]),
                    visit_expr(expr->expr()[1])
                )
            );
        else if(expr->PREUNIOP() && expr->expr().size() == 1)
            return std::make_shared<ast::uniop_expr_t>
            (
                ast::uniop_expr_t
                (
                    expression::str_to_uniop(expr->UNIOP()->getText()),
                    visit_expr(expr->expr()[0])
                )
            );
        else if(expr->UNIOP() && expr->expr().size() == 1)
        {
            if(expr->getStart()->getText() == expr->UNIOP()->getText())
                return std::make_shared<ast::uniop_expr_t>
                (
                    ast::uniop_expr_t
                    (
                        expression::str_to_uniop(expr->UNIOP()->getText()),
                        visit_expr(expr->expr()[0])
                    )
                );
            else
                return std::make_shared<ast::uniop_expr_t>
                (
                    ast::uniop_expr_t
                    (
                        visit_expr(expr->expr()[0]),
                        expression::str_to_uniop(expr->UNIOP()->getText())
                    )
                );
        }
        std::exit(exit::INVALID_STATE);
    }
}
