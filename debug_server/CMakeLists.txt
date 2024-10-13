cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(R-Type_network VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include_directories(./include)
# include_directories(./include/server)
# include_directories(./build/_deps/asio-src/include)

include(../src/cmake/CPM.cmake)

CPMAddPackage(
  NAME Boost
  VERSION 1.86.0
  URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
  OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_INCLUDE_LIBRARIES asio"
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})


# Add an executable for the server project
add_executable(udp_server
    Main.cpp
    UDPServer.cpp
)

target_link_libraries(udp_server
    Boost::asio
)
