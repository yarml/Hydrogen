cmake_minimum_required(VERSION 3.12.2)

set(DTC_TGT "")
add_subdirectory(DeuteriumC)

project("Hydrogen compiler")

# BEGIN configuration

set(HYC_SRC_DIR "src/") # Source file directory
set(HYC_INC_DIR "inc/") # Include directory
set(HYC_TGT     "hyc")

# END configuration

file(GLOB_RECURSE HYC_SRC "${HYC_SRC_DIR}*.c")
file(GLOB_RECURSE HYC_INC "${HYC_INC_DIR}*.c" "${HYC_SRC_DIR}*.h")

add_executable(${HYC_TGT} ${HYC_SRC} ${HYC_INC})

# Add HYC definition
target_compile_definitions(${HYC_TGT} PRIVATE HYC)


# Handle build in debug mode
if(DEBUG)
    message("Building in debug mode...")
    set(CMAKE_BUILD_TYPE Debug)
    target_compile_definitions(${HYC_TGT} PRIVATE HYC_DEBUG)
else()
    message("Building in release mode...")
    set(CMAKE_BUILD_TYPE Release)
    target_compile_definitions(${HYC_TGT} PRIVATE HYC_RELEASE)
endif()

# Include directories
target_include_directories(hyc PRIVATE "${HYC_INC_DIR}/")

target_link_libraries(${HYC_TGT} PRIVATE dtc)
