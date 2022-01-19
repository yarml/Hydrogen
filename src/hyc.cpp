#include <hyc.hpp>
#include <log.hpp>

namespace hyc
{
    void start(
        std::istream const& input_stream      ,
        std::ostream const& output_stream     ,
        int                 optimisation_level,
        logger::level       log_level
    )
    {
        logger::set_log_level(log_level);

        debug << "Start function called!" << logger::endm;
        verbose << "Optimisation level set to: " << optimisation_level                  << logger::endm;
        verbose << "Log level set to         : " << static_cast<std::size_t>(log_level) << logger::endm;
    }
}
