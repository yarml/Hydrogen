cmake_minimum_required(VERSION 3.21)

project("Hydrogen compiler")

# BEGIN configuration

set(HYC_SRC_DIR "src"    ) # Source file directory
set(HYC_INC_DIR "include") # Include directory

# END configuration

file(GLOB_RECURSE HYC_SRC "${HYC_SRC_DIR}/*.c")

add_executable(hyc ${HYC_SRC})

# Add HYC definition
target_compile_definitions(hyc PRIVATE HYC)


# Handle build in debug mode
if(RELEASE)
    message("Building in release mode...")
    set(CMAKE_BUILD_TYPE Release)
    target_compile_definitions(hyc PRIVATE HYC_RELEASE)
else()
    message("Building in debug mode...")
    set(CMAKE_BUILD_TYPE Debug)
    target_compile_definitions(hyc PRIVATE HYC_DEBUG)
endif()

# Use C++ 20
set_property(TARGET hyc PROPERTY C_STANDARD 17)

# Include directories
target_include_directories(hyc PRIVATE "${HYC_INC_DIR}/")

# Use spdlog
