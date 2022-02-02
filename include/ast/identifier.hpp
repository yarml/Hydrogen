#pragma once

#include <memory>
#include <vector>
#include <string>
#include <ast/node.hpp>
#include <log.hpp>

namespace hyc::ast
{
    
    struct qualified_id;

    using type_id = qualified_id;

    using qualified_id_ptr = std::unique_ptr<qualified_id>;
    using type_id_ptr = std::unique_ptr<type_id>;

    struct qualified_id : public node
    {
        std::vector<std::string> namespaces;
        std::string              identifier;

        friend hyc::logger& operator<<(hyc::logger&, qualified_id const&);
    };


    inline hyc::logger& operator<<(hyc::logger& l, qualified_id const& qid)
    {
        for(std::string const& ns : qid.namespaces)
                l << ns << "::";
        l << qid.identifier;
        return l;
    }
}
