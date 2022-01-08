#include <iostream>
#include <fstream>
#include <antlr4-runtime.h>
#include <HydrogenLexer.h>
#include <HydrogenParser.h>
#include <source_visitor.hpp>

int main(int argc, const char* argv[]) 
{
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <src_file>\n";
        return -1;
    }

    std::ifstream stream(argv[1]);
    if(!stream.is_open())
    {
        std::cout << "Couldn't open file: " << argv[1];
        return -2;
    }
    else
    {
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
        // END DBUG
        stream.open(argv[1]);
        antlr4::ANTLRInputStream input(stream);
        HydrogenLexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);

        std::cout << "---------- Parser ---------\n";

        HydrogenParser parser(&tokens);

        HydrogenParser::SourceContext* tree;
        tree = parser.source();
        
        hydrogen::source_visitor visitor;
        std::cout << visitor.visitSource(tree).as<std::string>();
    }
    std::cout << "Done!\n";
    return 0;
}
