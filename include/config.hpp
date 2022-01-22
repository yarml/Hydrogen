#pragma once

#include <log.hpp>

namespace hyc::config
{
    
    bool debug_mode();
    logger::level log_level();
    void configure(logger::level log_level, bool debug_mode);
}

