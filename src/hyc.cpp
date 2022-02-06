#include <hyc.hpp>
#include <log.hpp>
#include <lexer.hpp>

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

        lex::lexer l(input_stream);
        l.lex();
    }
}
