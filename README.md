# Hydrogen
Hydrogen language compiler

# Test branch
This branch is to try and handwrite a lexer and parser

If it is successful, then this will be merged with the main branch

# Build dependencies
`spdlog`

# TODO:
Write this page

# How to compile
After you clone the sources, you may want to edit the makefile(configurations worth modification are put at the top)

To generate the grammar files, run `make grammars`, then to compile everything: `make`

Running them both in one command `make grammars all` will not work for the moment, I will fix it later

Also make sure that you have `antlr4` and `antlr4-runtime` installed in your system with support for C++
