#pragma once

#include <HydrogenBaseVisitor.h>

namespace hydrogen
{
    class source_visitor : public HydrogenBaseVisitor
    {
    public:
        antlrcpp::Any visitSource(HydrogenParser::SourceContext* ctx) override;
        antlrcpp::Any visitPrint_stmt(HydrogenParser::Print_stmtContext* pr) override;
        antlrcpp::Any visitDataholder_def(HydrogenParser::Dataholder_defContext* dhd_ctx) override;
        antlrcpp::Any visitStmt(HydrogenParser::StmtContext* stmt) override;
        antlrcpp::Any visitType_spec(HydrogenParser::Type_specContext* tspec) override; 
        antlrcpp::Any visitExpr(HydrogenParser::ExprContext* expr) override;
    };
}
