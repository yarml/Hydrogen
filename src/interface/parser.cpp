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
            pqid->namespaces.push_back("__root__");
        else
            pqid->namespaces.push_back("__auto__"); // __auto__ means look in __this__, if not found then in __root__
        for(auto it = vec.begin(); it != vec.end() - 1; ++it)
            pqid->namespaces.push_back((*it)->getText());
        pqid->identifier = vec.back()->getText();
        populate_node(pqid.get(), qid);
        debug << "qualifiedid: ";
        debug << *pqid;
        return pqid;
    }
    static expr_ptr build_expression(HydrogenParser::ExpressionContext* expr);
    static int read_int(antlr4::tree::TerminalNode* node)
    {
        std::string str = node->getText();
        std::remove_if(str.begin(), str.end(), [](char c) { return c == '_'; });
        try
        {
            if(str.starts_with("0x"))
                return std::stoi(str.substr(2), nullptr, 16);
            if(str.starts_with("0c"))
                return std::stoi(str.substr(2), nullptr,  8);
            if(str.starts_with("0b"))
                return std::stoi(str.substr(2), nullptr,  2);
            return std::stoi(str, nullptr, 10);
        }
        catch(std::exception const& e)
        {
            parse_error(node, "Couldn't parse integer: " + str);
        }
    }
    static float read_float(antlr4::tree::TerminalNode* node)
    {
        std::string str = node->getText();
        try
        {
            return std::stof(str);
        }
        catch(std::exception const& e)
        {
            parse_error(node, "Couldn't parse float: " + str);
        }
    }
    static bool read_bool(antlr4::tree::TerminalNode* node)
    {
        std::string str = node->getText();
        if(str == "true")
            return true;
        if(str == "false")
            return false;
        parse_error(node, "Couldn't parse boolean: " + str);
    }
    static char read_escaped_char(char c, antlr4::tree::TerminalNode* blame)
    {
        switch(c)
        {
            case '\'': return '\'';
            case '"' : return '"' ;
            case '?' : return '?' ;
            case '\\': return '\\';
            case 'a' : return '\a';
            case 'b' : return '\b';
            case 'f' : return '\f';
            case 'n' : return '\n';
            case 'r' : return '\r';
            case 't' : return '\t';
            case 'v' : return '\v';
            default: parse_error(blame, std::string("Couldn't parse escaped char: '\\") + c + std::string("'"));
        }
    }
    static char read_char(antlr4::tree::TerminalNode* node)
    {
        std::string str = node->getText().substr(1, node->getText().size() - 2);
        if(str.size() == 1)
            return str[0];
        else if(str.size() == 2 && str[0] == '\\')
            return read_escaped_char(str[1], node);
        else
            parse_error(node, "Invalid char length: " + node->getText());
    }
    static std::string read_string(antlr4::tree::TerminalNode* node)
    {
        std::string str = node->getText().substr(1, node->getText().size() - 2);
        std::stringstream ss;
        for(auto it = str.begin(); it != str.end(); ++it)
            if(*it == '\\')
                ss << read_escaped_char(*(++it), node);
            else
                ss << *it;
        return ss.str();
    }
    static literal_ptr build_literal(HydrogenParser::LiteralContext* literal)
    {
        literal_ptr pl;
        if(literal->INT())
        {
            int_literal_ptr pil = std::make_unique<int_literal>();
            pil->literal_type = literal_expr_type::INT;
            pil->val = read_int(literal->INT());
            debug << pil->val;
            pl = std::move(pil);
        }
        else if(literal->FLOAT())
        {
            float_literal_ptr pfl = std::make_unique<float_literal>();
            pfl->literal_type = literal_expr_type::FLOAT;
            pfl->val = read_float(literal->FLOAT());
            debug << pfl->val;
            pl = std::move(pfl);
        }
        else if(literal->BOOL())
        {
            bool_literal_ptr pbl = std::make_unique<bool_literal>();
            pbl->literal_type = literal_expr_type::BOOL;
            pbl->val = read_bool(literal->BOOL());
            debug << pbl->val;
            pl = std::move(pbl);
        }
        else if(literal->CHR())
        {
            char_literal_ptr pcl = std::make_unique<char_literal>();
            pcl->literal_type = literal_expr_type::CHAR;
            pcl->val = read_char(literal->CHR());
            debug << pcl->val;
            pl = std::move(pcl);
        }
        else if(literal->STR())
        {
            str_literal_ptr psl = std::make_unique<str_literal>();
            psl->literal_type = literal_expr_type::STR;
            psl->val = read_string(literal->STR());
            debug << psl->val;
            pl = std::move(psl);
        }
//        pl->type = expr_type::ATOMIC;
        pl->atom_type = atomic_expr_type::LITERAL;
        populate_node(pl.get(), literal);
        return pl;
    }
    static func_call_ptr build_func_call(HydrogenParser::Func_callContext* fc)
    {
        func_call_ptr pfc = std::make_unique<func_call>();
//        pfc->type = expr_type::ATOMIC;
        pfc->atom_type = atomic_expr_type::FUNC_CALL;
        pfc->func_id = build_qualified_id(fc->qualified_id());
        if(fc->func_arg_seq())
            for(HydrogenParser::ExpressionContext* argexpr : fc->func_arg_seq()->expression())
                pfc->args.push_back(build_expression(argexpr));
        populate_node(pfc.get(), fc);
        return pfc;
    }
    static id_expr_ptr build_id_expr(HydrogenParser::Qualified_idContext* qid)
    {
        id_expr_ptr pidexpr = std::make_unique<id_expr>();
//        pidexpr->type = expr_type::ATOMIC;
        pidexpr->atom_type = atomic_expr_type::IDENTIFIER;
        pidexpr->identifier = build_qualified_id(qid);
        populate_node(pidexpr.get(), qid);
        debug << *(pidexpr->identifier);
        return pidexpr;
    }
    static std::string get_op(HydrogenParser::ExpressionContext* expr)
    {
#define GET_OP_HANDLE(op) if(expr->op()) return expr->op()->getText();
        GET_OP_HANDLE(postfix_op)
        GET_OP_HANDLE(prefix_op)
        GET_OP_HANDLE(prod_op)
        GET_OP_HANDLE(sum_op)
        GET_OP_HANDLE(bitshift_op)
        GET_OP_HANDLE(rel_comp_op)
        GET_OP_HANDLE(equality_op)
        GET_OP_HANDLE(BIT_AND)
        GET_OP_HANDLE(BIT_XOR)
        GET_OP_HANDLE(BIT_OR)
        GET_OP_HANDLE(AND)
        GET_OP_HANDLE(OR)
#undef GET_OP_HANDLE
        parse_error(expr, "Unknown operator in: " + expr->getText());
    }
    static expr_ptr build_expression(HydrogenParser::ExpressionContext* expr)
    {
        if(expr->literal())
            return build_literal(expr->literal());
        if(expr->func_call())
            return build_func_call(expr->func_call());
        if(expr->qualified_id())
            return build_id_expr(expr->qualified_id());
        auto exprs = expr->expression(); // why tf do they return a new vector each time smh
        if(expr->PARENL() && exprs.size() == 1 && expr->PARENR())
            return build_expression(exprs[0]);
        // we have a composite expression here, probably
        func_call_ptr pfc = std::make_unique<func_call>();
        pfc->atom_type = atomic_expr_type::FUNC_CALL;
        for(auto* subexpr : exprs)
            pfc->args.push_back(build_expression(subexpr));
        pfc->func_id = std::make_unique<qualified_id>();
        pfc->func_id->namespaces.push_back("__root__");
        pfc->func_id->identifier = "__operator__" + get_op(expr);
        populate_node(pfc->func_id.get(), expr);
        populate_node(pfc.get(), expr);
        debug << "operator call: " << *(pfc->func_id);
        return pfc;
    }
    static qualified_id_ptr build_qualified_id(HydrogenParser::Unqualified_idContext* uqid)
    {
        qualified_id_ptr pqid = std::make_unique<qualified_id>();
        pqid->identifier = uqid->getText();
        populate_node(pqid.get(), uqid);
        debug << "qualifiedid(unqualified): " << pqid->identifier << '\n';
        return pqid;
    }
    static qualified_id_ptr build_qualified_id(std::vector<std::string> const& namespaces, std::string id)
    {
        qualified_id_ptr pqid = std::make_unique<qualified_id>();
        pqid->identifier = id;
        std::copy(namespaces.begin(), namespaces.end(), std::back_inserter(pqid->namespaces));
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
//        pfunc_call->type = expr_type::ATOMIC;
        pfunc_call->atom_type = atomic_expr_type::FUNC_CALL;
        pfunc_call->func_id = std::make_unique<qualified_id>();
        std::copy(type->namespaces.begin(), type->namespaces.end(), std::back_inserter(pfunc_call->func_id->namespaces));
        pfunc_call->func_id->namespaces.push_back(type->identifier);
        pfunc_call->func_id->identifier = "__default__value__";
        debug << "Default expression: ";
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
            auto* sdecl = decl->strg_decl();
            decl_strg_ptr psdecl = std::make_unique<decl_strg>();
            psdecl->type      = build_qualified_id(sdecl->strg_sig()->type_id()->qualified_id());
            psdecl->scope     = build_scope(sdecl->strg_sig()->id_scope());
            psdecl->id        = build_qualified_id(sdecl->strg_sig()->unqualified_id());
            pdecl = std::move(psdecl);
        }
        else if(decl->strg_def())
        {
            debug << "START storage definition\n";
            auto* sdef = decl->strg_def();
            decl_strg_ptr psdef = std::make_unique<decl_strg>();
            psdef->type      = build_qualified_id(sdef->strg_sig()->type_id()->qualified_id());
            psdef->scope     = build_scope(sdef->strg_sig()->id_scope());
            psdef->id        = build_qualified_id(sdef->strg_sig()->unqualified_id());
            debug << "Assign expression: ";
            if(sdef->expression())
                psdef->default_value = build_expression(sdef->expression());
            else
                psdef->default_value = default_value(psdef->type);
            debug << '\n';
            pdecl = std::move(psdef);
        }
        else if(decl->func_decl())
        {
            debug << "START function declaration\n";
            auto* fsig = decl->func_decl()->func_sig();
            decl_func_ptr pfdecl = std::make_unique<decl_func>();
            pfdecl->scope = build_scope(fsig->id_scope());
            if(fsig->RETURNS())
                pfdecl->return_type = build_qualified_id(fsig->type_id()->qualified_id());
            else
                pfdecl->return_type = build_qualified_id({"__root__"}, "void");
            auto args = fsig->func_arg_decl_seq()->strg_sig();
            for(auto* arg : args)
            {
                
            }
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

