#include <iostream>
#include <exit.hpp>
#include <log.hpp>

namespace hyc::exit
{
    [[noreturn]]
    void exit(exit_code ec, std::string const& exit_msg, std::string const& verbose_msg)
    {
        if(!exit_msg.empty())
        {
            if(ec == 0)
                info << exit_msg     << logger::endm;
            else
                err  << exit_msg << logger::endm;
        }
        if(!verbose_msg.empty())
        verbose  << verbose_msg << logger::endm;
        debug    << "Exiting for reason: " << ec << logger::endm;
        std::exit(ec);
    }
}