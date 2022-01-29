#include <cstddef>
#include <argparse.hpp>
#include <exit.hpp>
#include <log.hpp>
#include <hyc.hpp>

// The main function is responsible for parsing the arguments and passing them to the actual compiler
// The main function is NOT responsible for checking if those arguments are valid except for their type
// that is for example it would only check if the optimisation level is actually a number,
// but it wouldn't check if it is between 0 and 3
int main(int argc, char** argv)
{
    argparse::ArgumentParser prog(hyc::ID.data(), hyc::VER.data());
    prog.add_argument("--output", "-o")
        .help("Set output file")
        .default_value<std::string>("a.out")
        .required();
    prog.add_argument("--optimisation", "-O")
        .help("Set optimisation level")
        .default_value<std::size_t>(0)
        .scan<'d', std::size_t>()
        .required();
    prog.add_argument("--log-level", "-ll")
        .help("Set log level")
        .default_value<std::string>("err")
        .required();
    prog.add_argument("--debug", "-d")
        .help("Enable debug mode")
        .default_value<bool>(false)
        .implicit_value(true)
        .required();
    prog.add_argument("filename")
        .help("Input file");
    try
    {
        prog.parse_args(argc, argv);
    }
    catch(std::runtime_error const& e)
    {
        hyc::err << e.what() << hyc::logger::endm;
        hyc::exit::exit(hyc::exit::INVALID_ARGS, "Error parsing arguments");
    }
    // Get arguments
    std::string file_name = prog.get<std::string>("filename");
    std::ifstream      input_file        ;
    std::ofstream      output_file       ;
    std::size_t        optimisation_level;
    std::string        log_level_str     ;
    hyc::logger::level log_level         ;
    bool               debug_mode = prog.get<bool>("--debug");
    try
    {
        input_file.open(     file_name                              );
        output_file.open(    prog.get<std::string>("--output"      ));
        optimisation_level = prog.get<std::size_t>("--optimisation") ;
        log_level_str      = prog.get<std::string>("--log-level"   ) ;
    }
    catch(std::exception const& e)
    {
        hyc::err << e.what() << hyc::logger::endm;
        hyc::exit::exit(hyc::exit::INVALID_ARGS, "Error parsing arguments");
    }
    // Check arguments
    if(!input_file.is_open())
        hyc::exit::exit(hyc::exit::FILE_ERROR, "Couldn't open input file");
    if(!output_file.is_open())
        hyc::exit::exit(hyc::exit::FILE_ERROR, "Couldn't open output file");
    
    if(log_level_str == "info")
        log_level = hyc::logger::level::INFO;
    else if(log_level_str == "warn")
        log_level = hyc::logger::level::WARN;
    else if(log_level_str == "err")
        log_level = hyc::logger::level::ERR;
    else if(log_level_str == "verbose")
        log_level = hyc::logger::level::VERBOSE;
    else if(log_level_str == "debug")
        log_level = hyc::logger::level::DEBUG;
    else if(log_level_str == "none")
        log_level = hyc::logger::level::NONE;
    else
        hyc::exit::exit(hyc::exit::INVALID_ARGS, "Unknown log level: " + log_level_str);

    // Start hyc
    hyc::start(file_name, input_file, output_file, optimisation_level, log_level, debug_mode);
}
