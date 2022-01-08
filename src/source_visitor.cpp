#include <source_visitor.hpp>
#include <sstream>

namespace hydrogen
{
    antlrcpp::Any source_visitor::visitSource(HydrogenParser::SourceContext* ctx)
    {
        std::stringstream ss;
        for(HydrogenParser::StmtContext* stmt : ctx->stmt())
            ss << visitStmt(stmt).as<std::string>();
        return ss.str();
    }
    antlrcpp::Any source_visitor::visitStmt(HydrogenParser::StmtContext* stmt)
    {
        std::stringstream ss;
        if(stmt->dataholder_def())
            ss << visitDataholder_def(stmt->dataholder_def()).as<std::string>();
        if(stmt->print_stmt())
            ss << visitPrint_stmt(stmt->print_stmt()).as<std::string>();
        return ss.str();
    }
    antlrcpp::Any source_visitor::visitPrint_stmt(HydrogenParser::Print_stmtContext* pr)
    {
        std::stringstream ss;
        ss << "Print stament: \n";
        ss << "\tExpression: " << visitExpr(pr->expr()).as<std::string>() << '\n';
        return ss.str();
    }
    antlrcpp::Any source_visitor::visitDataholder_def(HydrogenParser::Dataholder_defContext* dhd_ctx)
    {
        std::stringstream ss;
        std::string scope  = "implicit(local)";
        std::string def_value = "Unspecified";
        if(dhd_ctx->expr())
            def_value = visitExpr(dhd_ctx->expr()).as<std::string>();
        if(dhd_ctx->scope_spec())
            scope = dhd_ctx->scope_spec()->getText();
        ss << "Data holder definition: \n";
        ss << "\tScope               : " << scope << '\n';
        ss << "\tData type           : " << visitType_spec(dhd_ctx->type_spec()).as<std::string>() << '\n';
        ss << "\tName                : " << dhd_ctx->ID()->getText() << '\n';
        ss << "\tDefault value       : " << def_value << '\n';
        return ss.str();
    }
    antlrcpp::Any source_visitor::visitType_spec(HydrogenParser::Type_specContext* tspec)
    {
        std::stringstream ss;
        
        if(tspec->dataholder_type_spec())
            ss << tspec->dataholder_type_spec()->getText() << ' ';
        else
            ss << "implicit(const) ";
        ss << tspec->type_id()->ID()->getText();
        if(tspec->type_id()->type_spec())
            ss << '<' << visitType_spec(tspec->type_id()->type_spec()).as<std::string>() << '>';
        return ss.str();
    }
    antlrcpp::Any source_visitor::visitExpr(HydrogenParser::ExprContext* expr)
    {
        std::stringstream ss;
        if(expr->NUM())
            ss << "NUM(" << expr->NUM()->getText() << ')';
        else if(expr->STR())
            ss << "STR(" << expr->STR()->getText().substr(1, expr->STR()->getText().size() - 2) << ')';
        else if(expr->ID())
            ss << "ID(" << expr->ID()->getText() << ')';
        else if(expr->PARENL() && expr->expr().size() == 1 && expr->PARENR())
            ss << visitExpr(expr->expr()[0]).as<std::string>();
        else if(expr->BINOP() && expr->expr().size() == 2)
            ss << "BINOP(" << expr->BINOP()->getText() << ','
               << visitExpr(expr->expr()[0]).as<std::string>() << ','
               << visitExpr(expr->expr()[1]).as<std::string>() << ')';
        else if(expr->PREUNIOP() && expr->expr().size() == 1)
            ss << "PREOP(" << expr->PREUNIOP() << ','
               << visitExpr(expr->expr()[0]).as<std::string>() << ')';
        else if(expr->UNIOP() && expr->expr().size() == 1)
        {
            if(expr->getStart()->getText() == expr->UNIOP()->getText())
                ss << "PREOP(" << expr->UNIOP()->getText() << ','
                   << visitExpr(expr->expr()[0]).as<std::string>() << ')';
            else
                ss << "POSTOP(" << expr->UNIOP()->getText() << ','
                   << visitExpr(expr->expr()[0]).as<std::string>() << ')';
        }
        else
            std::cout << "Idk man, life sucks sometimes\n";
        return ss.str();
    }
}

