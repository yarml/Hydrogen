#include <log.hpp>

namespace hyc
{
    logger::logger(level log_level, std::string const& prefix)
        : m_prefix(prefix),
          m_log_level(log_level),
          m_new_msg(true)
    { }
    void logger::flush()
    {
        std::cout << std::flush;
    }

    logger::smsg const logger::endm = logger::smsg::END_MSG;

    template<>
    void logger::write<logger::smsg>(logger::smsg const& s_msg)
    {
        if(static_cast<std::size_t>(m_log_level) <= static_cast<std::size_t>(config::log_level()))
        {
            switch(s_msg)
            {
            case smsg::END_MSG:
                std::cout << '\n';
                m_new_msg = true;
                break;
            }
        }
    }

    logger info(   logger::level::INFO   , "info: "   );
    logger note(   logger::level::NOTE   , "note: "   );
    logger warn(   logger::level::WARN   , "warn: "   );
    logger err(    logger::level::ERR    , "error: "  );
    logger verbose(logger::level::VERBOSE, "verbose: ");
    logger debug(  logger::level::DEBUG  , "debug: "  );
}
