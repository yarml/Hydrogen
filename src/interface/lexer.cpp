#include <interface/lexer.hpp>
#include <ANTLRInputStream.h>
#include <HydrogenLexer.h>

#include <config.hpp>

namespace hyc
{
    static void lexer_print_debug(HydrogenLexer& lexer, token_stream& ts)
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
        ts->seek(0);
    }
    token_stream lex(std::istream& istream)
    {
        antlr4::ANTLRInputStream input(istream);
        HydrogenLexer lexer(&input);
        token_stream ts = std::make_unique<token_stream::element_type>(&lexer);
        if(config::debug_mode())
            lexer_print_debug(lexer, ts);
        return ts;
    }
}
