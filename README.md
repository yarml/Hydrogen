# Hydrogen
## Why was it created?
A lot of factors made me decide creating Hydrogen, the first one being my lack of knowledge in C++'s undefined behaviours, which makes me 60% of the time trying to fight the compiler to compile something which looks fine to me

A second reason is to write my operating system [HeliumOS] with it, since C++ is mainly designed for high level application programming, it just feels weird writing an operating system in C++, the entire lore of C++ is in the STL which can't be used in osdev, and at the same time it has a lot of cool features that C lacks, so I don't want to just write the OS in C

# Build Hydrogen
## Clone the sources
Hydrogen is currently only hosted on Github

From a terminal, it can be cloned with: `git clone https://github.com/YavaCoco/Hydrogen.git`

## File structure
The file structure is very straight forward:

[inlude]: Contains the headers for Hydrogen

[src]: Contains the source files for Hydrogen

[libs]: Contains header only libraries that are used by Hydrogen

## The build system
Hydrogen comes with a [CMakeFiles.txt] to build the project

## Dependecies
Currently, Hydrogen depends on:

`spdlog`

## Building
Since Hydrogen uses `cmake` the build process is really easy

Assuming you are currently in the root folder for Hydrogen

Make a build directory: `mkdir build`

Change directory to the build folder: `cd build`

Generate Makefiles: `cmake ..`

Run make: `make`

The file `hyc` is you output Hydrogen compiler binary

## Running hyc
Basic usage: `./hyc <input_file> -o <output_file>`

To enable debug output(Where the most interesting stuff happens), you can use the `-cd` flag:
`./hyc <input_file> -cd`

To check the version of the binary use: `./hyc -v` or `./hyc --version`

All the other command line arguments can be accessed with: `./hyc -h` or `./hyc --help`

[include]: https://github.com/YavaCoco/Hydrogen/tree/main/include
[libs]: https://github.com/YavaCoco/Hydrogen/tree/main/libs
[src]: https://github.com/YavaCoco/Hydrogen/tree/main/src
[CMakeFiles.txt]: https://github.com/YavaCoco/Hydrogen/tree/main/CMakeFiles.txt
[HeliumOS]: https://github.com/YavaCoco/HeliumOS
