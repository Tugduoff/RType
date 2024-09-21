# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(MyProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the binary output directory to a custom path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include the subdirectories for client and server
add_subdirectory(client)
add_subdirectory(server)

# add dependencies
include(cmake/CPM.cmake)

# import SFML Dependencies
CPMAddPackage("gh:xiph/flac#1.4.3")

# import SFML
CPMAddPackage("gh:SFML/SFML#2.6.1")

