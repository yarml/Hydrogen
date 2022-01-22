#pragma once

#include <iostream>
#include <memory>
#include <HydrogenLexer.h>
#include <CommonTokenStream.h>

namespace hyc
{
    using token_stream = std::unique_ptr<antlr4::CommonTokenStream>;

    token_stream lex(std::istream& istream);
}
