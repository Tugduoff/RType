# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Enable config export to clangd language server
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# May be needed
if (WIN32)
    set(MSVC True)
endif (WIN32)

# Set the project name and version
project(RTypePlugins VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the binary output directory to a custom path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release)

# Add some addiotionnal flags to pass to compiler
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    message("Debug build, adding following compiler flags : -g -Wall -Wextra")
    add_definitions(-g -Wall -Wextra)
endif()

# Add the include directory
include_directories(./)
# To acces plugin include directory
include_directories(../)
# To allow plugin files include to work
include_directories(../src)

include_directories(./build/_deps/libconfig-src/lib)

include(../src/cmake/CPM.cmake)
CPMAddPackage(
    NAME libconfig
    GIT_REPOSITORY "https://github.com/hyperrealm/libconfig.git"
    GIT_TAG "v1.7.3"
    OPTIONS
    "BUILD_EXAMPLES OFF"
    "BUILD_TESTS OFF"
)

if (WIN32)
    set(LIBCONFIG_LIB_NAME libconfig libconfig++)
else()
    set(LIBCONFIG_LIB_NAME config config++)
endif (WIN32)

# Include the subdirectories for client and server
add_subdirectory(components)
add_subdirectory(systems)
