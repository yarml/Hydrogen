#include <iostream>
#include <fstream>
#include <antlr4-runtime.h>
#include <HydrogenLexer.h>
#include <HydrogenParser.h>
#include <debug_visitor.hpp>
#include <hydrogen_visitor.hpp>
#include <exit_reason.hpp>
#include <cstdlib>
#include <interpreter.hpp>


#define HY_DEBUG 0

int main(int argc, const char* argv[]) 
{
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <src_file>\n";
        std::exit(hydrogen::exit::INVALID_USAGE);
    }

    std::ifstream stream(argv[1]);
    if(!stream.is_open())
    {
        std::cout << "Couldn't open file: " << argv[1];
        std::exit(hydrogen::exit::FILE_OPEN_ERROR);
    }
    else
    {
#if HY_DEBUG == 1
        // DEBUG
        std::cout << "---------- Lexer ----------\n";
        stream.close();
        std::ifstream stream2(argv[1]);
        antlr4::ANTLRInputStream input2(stream2);
        HydrogenLexer lexer2(&input2);
        antlr4::CommonTokenStream tokens2(&lexer2);
        tokens2.fill();
        antlr4::dfa::Vocabulary const& vocab = lexer2.getVocabulary();
        for(antlr4::Token* t : tokens2.getTokens())
        {
            std::string rule_name;
            std::string rule_txt = t->getText();
            if(rule_txt == "\n") rule_txt = "\\n";
            else if(rule_txt == "\t") rule_txt = "\\t";
            else if(rule_txt == "\r") rule_txt = "\\r";
            
            rule_name = vocab.getSymbolicName(t->getType());

            std::cout << rule_name << '(' << rule_txt << ')' << ' ';
        }
        std::cout << '\n';
        stream2.close();
        stream.open(argv[1]);
        // END DBUG
#endif
        antlr4::ANTLRInputStream input(stream);
        HydrogenLexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);


        HydrogenParser parser(&tokens);
        HydrogenParser::SourceContext* tree;
        tree = parser.source();
#if HY_DEBUG == 1
        hydrogen::debug_visitor d_visitor;
        std::cout << "---------- Parser ---------\n";
        std::cout << d_visitor.visitSource(tree).as<std::string>();
#endif
        hydrogen::hydrogen_visitor visitor;
        auto src = visitor.visit_source(tree);
        hydrogen::interpreter<0> a;
        a.interpret(src);
    }
#if HY_DEBUG == 1
    std::cout << "Done!\n";
#endif
    std::exit(hydrogen::exit::SUCCESS);
}
