#include <hyc.hpp>
#include <log.hpp>
#include <lexer.hpp>
#include <chrono>

namespace hyc
{
    void start(
        std::string const& file_name         ,
        std::istream&      input_stream      ,
        std::ostream&      output_stream     ,
        int                optimisation_level,
        bool               debug_mode
    )
    {
        spdlog::debug("hyc with filename={} opt_level={} debug_mode={}", file_name, optimisation_level, debug_mode);
        
        lex::lexer l(input_stream, file_name);
        std::vector<lex::token> tokens = l.lex();
        for(lex::token const& t : tokens)
            spdlog::debug("{} at {}:{}: {}", t.get_type(), t.get_line(), t.get_cpos(), t.get_text());
    }
}
