# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# May be needed
if (WIN32)
    set(MSVC True)
endif (WIN32)
# set( CMAKE_CXX_COMPILER "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Tools\\MSVC\\14.41.34120\\bin\\Hostx64\\x64\\cl.exe" )
# set( CMAKE_C_COMPILER "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Tools\\MSVC\\14.41.34120\\bin\\Hostx64\\x64\\cl.exe" )

# Set the project name and version
project(R-Type VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BINARY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/../bin)

# Set the binary output directory to a custom path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARY_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARY_OUTPUT_DIR}/release)

# Add some addiotionnal flags to pass to compiler
if(CMAKE_BUILD_TYPE EQUAL Debug AND NOT WIN32)
    message("Debug build, adding following compiler flags : -g -Wall -Wextra")
    add_definitions(-g -Wall -Wextra)
endif()

# Add the include directory
include_directories(./)
# To acces plugin include directory
include_directories(../)
# To allow plugin files include to work
include_directories(../plugins)

include_directories(./build/_deps/libconfig-src/lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(./cmake/CPM.cmake)
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
add_subdirectory(client)
add_subdirectory(server)
