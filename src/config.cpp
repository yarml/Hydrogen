#include <config.hpp>

namespace hyc::config
{
    static bool          s_debug_mode = false               ;
    static logger::level s_log_level  = logger::level::DEBUG;

    bool debug_mode()
    {
        return s_debug_mode;
    }

    logger::level log_level()
    {
        return s_log_level;
    }

    void configure(logger::level log_level, bool debug_mode)
    {
        s_debug_mode = debug_mode;
        s_log_level  = log_level ;
    }
}   

