#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace hyc
{
    class logger
    {
    public:
        enum class level
        {
            NONE   ,
            ERR    ,
            WARN   ,
            NOTE   ,
            INFO   ,
            VERBOSE,
            DEBUG
        };
    private:
        enum class smsg
        {
            END_MSG
        };
    public:
        logger(level log_level, std::string const& prefix);
    public:
        void flush();
        
        template <typename T>
        void write(T const& t);
    public:
        template<typename T>
        logger& operator<<(T const& t)
        {
            write(t);
            return *this;
        }
    private:
        std::string m_prefix   ;
        level       m_log_level;
        bool        m_new_msg  ;
    public:
        smsg static const endm;
    };
    template<>
    void logger::write<logger::smsg>(logger::smsg const& s_msg);


    extern logger info   ;
    extern logger note   ;
    extern logger warn   ;
    extern logger err    ;
    extern logger verbose;
    extern logger debug  ;
}

namespace hyc::config
{
    logger::level log_level();
}

namespace hyc
{
    template <typename T>
    void logger::write(T const& t)
    {
        if(static_cast<std::size_t>(m_log_level) <= static_cast<std::size_t>(config::log_level()))
        {
            if(m_new_msg)
            {
                m_new_msg = false;
                std::cout << m_prefix;
            }
            std::cout << t;
        }
    }
}