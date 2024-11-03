cmake_minimum_required(VERSION 3.10)
project(level_editor)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the binary output directories
set(BINARY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/../bin)

# Set the binary output directory to a custom path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARY_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARY_OUTPUT_DIR}/release)

# Find and link libconfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

# Define the executable and include Gui.cpp
add_executable(level_editor Main.cpp Gui.cpp)

# Link the libconfig library
target_include_directories(level_editor PRIVATE ${LIBCONFIG_INCLUDE_DIRS})
target_link_libraries(level_editor
    raylib
    ${LIBCONFIG_LIBRARIES})
