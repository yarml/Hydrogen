#include <interface/lexer.hpp>
#include <ANTLRInputStream.h>
#include <HydrogenLexer.h>

#include <config.hpp>

namespace hyc
{
    std::unique_ptr<HydrogenLexer> s_plexer;
    std::unique_ptr<antlr4::ANTLRInputStream> s_pinputs;
    static void lexer_print_debug(HydrogenLexer& lexer, token_stream_ptr& ts)
    {
        ts->fill();
        antlr4::dfa::Vocabulary const& vocab = lexer.getVocabulary();
        verbose << "Lexer output:\n-------------------------\n";
        for(antlr4::Token* t : ts->getTokens())
        {
            std::string rule_name = vocab.getSymbolicName(t->getType());
            std::string rule_text = t->getText();
            if(rule_text == "\n") rule_text = "\\n";
            else if(rule_text == "\t") rule_text = "\\t";
            else if(rule_text == "\r") rule_text = "\\r";
            verbose << rule_name << '(' << rule_text << ')' << ' ';
        }
        verbose << "\n-------------------------" << logger::endm;
        ts->reset();
    }
    token_stream_ptr lex(std::istream& istream)
    {
        s_pinputs = std::make_unique<antlr4::ANTLRInputStream>(istream);
        s_plexer = std::make_unique<HydrogenLexer>(s_pinputs.get());
        token_stream_ptr ts = std::make_unique<token_stream>(s_plexer.get());
        if(config::debug_mode())
            lexer_print_debug(*s_plexer, ts);
        return ts;
    }
}
