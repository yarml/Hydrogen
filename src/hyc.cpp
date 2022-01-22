#include <hyc.hpp>
#include <log.hpp>
#include <config.hpp>

#include <interface/lexer.hpp>

namespace hyc
{
    void start(
        std::string const& file_name         ,
        std::istream&      input_stream      ,
        std::ostream&      output_stream     ,
        int                optimisation_level,
        logger::level      log_level         ,
        bool               debug_mode
    )
    {
        config::configure(log_level, debug_mode);
        debug   << "start function called!"                                             << logger::endm;
        verbose << "Input file name          : " << file_name                           << logger::endm;
        verbose << "Optimisation level set to: " << optimisation_level                  << logger::endm;
        verbose << "Log level set to         : " << static_cast<std::size_t>(log_level) << logger::endm;
        verbose << "Debug mode               : " << debug_mode                          << logger::endm;
        
        token_stream ts = lex(input_stream);
        
    }
}
