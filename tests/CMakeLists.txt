cmake_minimum_required(VERSION 3.29)

project(rtype_unit_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./Zipper)
include_directories(../src)

# Add an executable for the server project
add_executable(tests
    zipper_tests.cpp
    indexed_zipper_tests.cpp
)

# Set the output name for the server binary
set_target_properties(tests PROPERTIES OUTPUT_NAME "unit_tests")

target_link_libraries(tests
    criterion
)
