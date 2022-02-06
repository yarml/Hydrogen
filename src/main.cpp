#include <argparse.hpp>
#include <log.hpp>
#include <hyc.hpp>

#include <fstream>

int main(int argc , char** argv)
{
    spdlog::set_pattern("[%^%l%$] %v");
    argparse::ArgumentParser prog(hyc::ID.data(), hyc::VER.data());
    prog.add_argument("--output", "-o")
        .help("Specify output file")
        .default_value<std::string>("a.out")
        .required();
    prog.add_argument("--optimisation", "-O")
        .help("Specify optimisation level")
        .default_value<std::size_t>(0)
        .scan<'d', std::size_t>()
        .required();
    prog.add_argument("--debug", "-d")
        .help("Enable debug mode")
        .default_value<bool>(false)
        .implicit_value(true)
        .required();
    prog.add_argument("--cdebug", "-cd")
        .help("Output compiler debug information")
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
        spdlog::error("Error parsing arguments: {}", e.what());
        std::exit(hyc::exit::INVALID_ARGS);
    }
    std::string file_name  ;
    std::string output_file_name;
    std::size_t opt_level  ;
    bool        debug_mode ;
    try
    {
        file_name        = prog.get<std::string>("filename"      );
        output_file_name = prog.get<std::string>("--output"      );
        opt_level        = prog.get<std::size_t>("--optimisation");
        debug_mode       = prog.get<bool>(       "--debug"       );
        if(prog.get<bool>("--cdebug"))
            spdlog::set_level(spdlog::level::trace); // log everything
    }
    catch(std::exception const& e)
    {
        spdlog::error("Error parsing arguments: {}", e.what());
        std::exit(hyc::exit::INVALID_ARGS);   
    }
    std::ifstream input_file ;
    std::ofstream output_file;

    input_file.open( file_name       );
    output_file.open(output_file_name);
    // Check arguments
    if(!input_file.is_open())
    {
        spdlog::error("Couldn't open file: {}", file_name);
        std::exit(hyc::exit::FILE_ERROR);
    }
    if(!output_file.is_open())
    {
        spdlog::error("Couldn't open file: {}", output_file_name);
        std::exit(hyc::exit::FILE_ERROR);
    }
    

    // Start hyc
    hyc::start(file_name, input_file, output_file, opt_level, debug_mode);
}
