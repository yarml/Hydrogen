#include <interface/parser.hpp>
#include <HydrogenParser.h>
#include <memory>
#include <sstream>
#include <exit.hpp>
#include <ast/expression.hpp>
#include <log.hpp>

namespace hyc
{
    using namespace ast;
    // static fields
    static std::unique_ptr<HydrogenParser> s_pparser;
    //
    [[noreturn]]
    static void parse_error(std::size_t line, std::size_t cpos, std::string const& msg)
    {
        std::stringstream ss;
        ss << "Parse error at: " << line << ':' << cpos << ": " << msg;
        exit::exit(exit::PARSE_EROOR, ss.str());
    }
    [[noreturn]]
    static void parse_error(antlr4::ParserRuleContext* ctx, std::string const& msg)
    {
        parse_error(ctx->getStart()->getLine(), ctx->getStart()->getCharPositionInLine(), msg);
    }
    [[noreturn]]
    static void parse_error(antlr4::tree::TerminalNode* nn, std::string const& msg)
    {
        parse_error(nn->getSymbol()->getLine(), nn->getSymbol()->getCharPositionInLine(), msg);
    }
    // build the AST
    static void populate_node(node* n, antlr4::ParserRuleContext* ctx)
    {
        n->cpos = ctx->getStart()->getCharPositionInLine();
        n->line = ctx->getStart()->getLine();
    }
    static void populate_node(node* n, antlr4::tree::TerminalNode* nn)
    {
        n->cpos = nn->getSymbol()->getCharPositionInLine();
        n->line = nn->getSymbol()->getLine();
    }
    static qualified_id_ptr build_qualified_id(HydrogenParser::Qualified_idContext* qid)
    {
        qualified_id_ptr pqid = std::make_unique<qualified_id>();
        auto vec = qid->unqualified_id();
        if(qid->global_namespace)
            pqid->namespaces.push_back("__global__");
        else
            pqid->namespaces.push_back("__this__");
        for(auto it = vec.begin(); it != vec.end() - 1; ++it)
            pqid->namespaces.push_back((*it)->getText());
        pqid->identifier = vec.back()->getText();
        populate_node(pqid.get(), qid);
        debug << "qualifiedid: ";
        if(config::log_level() == logger::level::DEBUG)
            for(std::string const& ns : pqid->namespaces)
                debug << ns << "::";
        debug << pqid->identifier << '\n';
        return pqid;
    }
    static expr_ptr build_expression(HydrogenParser::ExpressionContext* expr)
    {
        debug << "Building expression ya3ni\n";
        return nullptr;
    }
    static qualified_id_ptr build_qualified_id(HydrogenParser::Unqualified_idContext* uqid)
    {
        qualified_id_ptr pqid = std::make_unique<qualified_id>();
        pqid->identifier = uqid->getText();
        populate_node(pqid.get(), uqid);
        debug << "qualifiedid(unqualified specified): " << pqid->identifier << '\n';
        return pqid;
    }
    static scope_ptr build_scope(HydrogenParser::Id_scopeContext* idscope)
    {
        scope_ptr s = std::make_unique<scope>();
        if(idscope)
        {
            if(idscope->GLOBAL())
            {
                s->type = scope_type::GLOBAL;
                debug << "scope: GLOBAL\n";
            }
            else if(idscope->LOCAL())
            {
                s->type = scope_type::LOCAL;
                debug << "scope: LOCAL\n";
            }
            else
                parse_error(idscope, "Unknown scope specifier: " + idscope->getText());
            populate_node(s.get(), idscope);
        }
        else
        {
            s->type = scope_type::LOCAL;
            debug << "scope: (unspecified)LOCAL\n";
        }
        return s;
    }
    static decl_type_ptr build_decl_type(HydrogenParser::DeclContext* decl)
    {
        decl_type_ptr pdecl_type = std::make_unique<decl_type>();
        if(decl->DECL())
        {
            pdecl_type->type = decl_type_e::DECL;
            populate_node(pdecl_type.get(), decl->DECL());
            debug << "decl type: DECL\n";
        }
        else if(decl->DEF())
        {
            pdecl_type->type = decl_type_e::DEF;
            populate_node(pdecl_type.get(), decl->DEF());
            debug << "decl type: DEF\n";
        }
        else // declarations which do not specify decl_type are always definitions
        {
            pdecl_type->type = decl_type_e::DEF;
            debug << "decl type: (unspecified)DEF\n";
        }
        return pdecl_type;
    }
    static expr_ptr default_value(type_id_ptr& type)
    {
        func_call_ptr pfunc_call = std::make_unique<func_call>();
        pfunc_call->type = expr_type::ATOMIC;
        pfunc_call->atom_type = atomic_expr_type::FUNC_CALL;
        pfunc_call->func_id = std::make_unique<qualified_id>();
        std::copy(type->namespaces.begin(), type->namespaces.end(), std::back_inserter(pfunc_call->func_id->namespaces));
        pfunc_call->func_id->namespaces.push_back(type->identifier);
        pfunc_call->func_id->identifier = "__default__value__";
        debug << "Default expression: ";
        if(config::log_level() == logger::level::DEBUG)
            debug << *(pfunc_call->func_id);
        debug << "()\n";
        return pfunc_call;
    }
    // Does not handle empty declarations!
    static decl_ptr build_decl(HydrogenParser::DeclContext* decl)
    {
        decl_ptr pdecl;
        if(decl->strg_decl())
        {
            debug << "START storage declaration\n";
            auto sdecl = decl->strg_decl();
            decl_strg_ptr psdecl = std::make_unique<decl_strg>();
            psdecl->type      = build_qualified_id(sdecl->strg_sig()->type_id()->qualified_id());
            psdecl->scope     = build_scope(sdecl->strg_sig()->id_scope());
            psdecl->id        = build_qualified_id(sdecl->strg_sig()->unqualified_id());
            pdecl = std::move(psdecl);
        }
        else if(decl->strg_def())
        {
            debug << "START storage definition\n";
            auto sdef = decl->strg_def();
            decl_strg_ptr psdef = std::make_unique<decl_strg>();
            psdef->type      = build_qualified_id(sdef->strg_sig()->type_id()->qualified_id());
            psdef->scope     = build_scope(sdef->strg_sig()->id_scope());
            psdef->id        = build_qualified_id(sdef->strg_sig()->unqualified_id());
            if(sdef->expression())
                psdef->default_value = build_expression(sdef->expression());
            else
                psdef->default_value = default_value(psdef->type);
            pdecl = std::move(psdef);
        }
        else
            parse_error(decl, "Unknown declaration type in: " + decl->getText());
        pdecl->decl_type = build_decl_type(decl);
        debug << "END declaration/definition\n";
        return pdecl;
    }
    static tree_ptr build_source(HydrogenParser::SourceContext* src)
    {
        tree_ptr ptree = std::make_unique<tree>();
        debug << "START source:\n";
        for(auto decl : src->decl_seq()->decl())
            ptree->decls.push_back(build_decl(decl));
        debug << "END source" << logger::endm;
        return ptree;
    }
    // parser
    tree_ptr parse(token_stream_ptr& ts)
    {
        debug << "parsing...." << logger::endm;
        s_pparser = std::make_unique<HydrogenParser>(ts.get());
        return build_source(s_pparser->source());
    }
}

