#pragma once

#include <iostream>
#include <memory>
#include <HydrogenLexer.h>
#include <CommonTokenStream.h>

namespace hyc
{
    using token_stream     = antlr4::CommonTokenStream;
    using token_stream_ptr = std::unique_ptr<antlr4::CommonTokenStream>;

    token_stream_ptr lex(std::istream& istream);
}
