#include <utils.hpp>
#include <string>

namespace hydrogen
{
    int string_to_int(std::string str)
    {
        if(str.starts_with("0b") || str.starts_with("0B"))
            return std::stoi(str.substr(2), nullptr, 2);
        if(str.starts_with("0c") || str.starts_with("0C"))
            return std::stoi(str.substr(2), nullptr, 8);
        if(str.starts_with("0x") || str.starts_with("0X"))
            return std::stoi(str.substr(2), nullptr, 16);
        return std::stoi(str);
    }
}
