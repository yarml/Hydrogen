#pragma once

#include <HydrogenBaseVisitor.h>
#include <ast.hpp>

namespace hydrogen
{
    class hydrogen_visitor : public HydrogenBaseVisitor
    {
    public:
        ast::source                 visit_source(HydrogenParser::SourceContext                            * src     );
        ast::stmt                   visit_stmt(HydrogenParser::StmtContext                                * stmt    );
        ast::print_stmt             visit_print_stmt(HydrogenParser::Print_stmtContext                    * prnt    );
        ast::dhdef_stmt             visit_dataholder_def(HydrogenParser::Dataholder_defContext            * dhdef   );
        data_holder::type_spec      visit_type_spec(HydrogenParser::Type_specContext                      * tspec   );
        data_holder::container_type visit_dataholder_type_spec(HydrogenParser::Dataholder_type_specContext* dht_spec);
        data_holder::scope          visit_scope_spec(HydrogenParser::Scope_specContext                    * sspec   );
        ast::expr                   visit_expr(HydrogenParser::ExprContext                                * expr    );
    };
}

