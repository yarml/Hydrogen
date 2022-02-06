# Hydrogen
Hydrogen language compiler

# Test branch
This branch is to try and handwrite a lexer and parser

If it is successful, then this will be merged with the main branch

# Build dependencies
`spdlog`

# Clone this branch
`git clone https://github.com/YavaCoco/Hydrogen.git`

`cd Hydrogen`

`git checkout test`

Done

# Build instruction
Assuming that you are in the repository

`mkdir build`

`cd build`

`cmake ..`

`make`

Done. the executable `hyc` is your output program

Check `./hyc -h` or `./hyc --help` for usage

The basic usage is `./hyc <filename>`

maybe `./hyc <filename> -o <outputfile>` if you want to specify an output file

The current version doesn't even output to a file yet so...

# Test

The folder `examples` contains some made for testing files, except `draft.hy`

`input.hy` currently is to test the readnext prelexer function

# TODO:
Write this page

# How to compile
After you clone the sources, you may want to edit the makefile(configurations worth modification are put at the top)

To generate the grammar files, run `make grammars`, then to compile everything: `make`

Running them both in one command `make grammars all` will not work for the moment, I will fix it later

Also make sure that you have `antlr4` and `antlr4-runtime` installed in your system with support for C++
