cmake_minimum_required(VERSION 3.12.2)

project("Hydrogen compiler")

# BEGIN configuration

set(HYC_SRC_DIR "src"    ) # Source file directory
set(HYC_INC_DIR "include") # Include directory
set(HYC_LIB_DIR "libs"   ) # libraries directory

# END configuration

file(GLOB_RECURSE HYC_SRC "${HYC_SRC_DIR}/*.cpp")

add_executable(hyc ${HYC_SRC})

# Add HYC definition
target_compile_definitions(hyc PRIVATE HYC)


# Handle build in debug mode
if(DEBUG)
    message("Building in debug mode...")
    set(CMAKE_BUILD_TYPE Debug)
    target_compile_definitions(hyc PRIVATE HYC_DEBUG)
else()
    message("Building in release mode...")
    set(CMAKE_BUILD_TYPE Release)
    target_compile_definitions(hyc PRIVATE HYC_RELEASE)
endif()

# Use C++ 20
set_property(TARGET hyc PROPERTY CXX_STANDARD 20)

# Include directories
target_include_directories(hyc PRIVATE "${HYC_INC_DIR}/")
target_include_directories(hyc PRIVATE "${HYC_LIB_DIR}/${HYC_INC_DIR}")

# Use spdlog
find_package(spdlog REQUIRED)

target_link_libraries(hyc PRIVATE spdlog::spdlog)
