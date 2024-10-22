# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Project details
project(R-Type VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directories
set(BINARY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARY_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARY_OUTPUT_DIR}/release)

# Debug flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    message("Debug build, adding following compiler flags : -g -Wall -Wextra")
    add_definitions(-g -Wall -Wextra)
endif()

# Include directories
include_directories(./ ../ ./build/_deps/libconfig-src/lib)

# Download and include CPM.cmake
include(FetchContent)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")

# Télécharge CPM si non présent dans le dossier build/_deps/
if (NOT EXISTS "${CPM_DOWNLOAD_LOCATION}")
    message(STATUS "Téléchargement de CPM.cmake dans ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION}
    )
endif()

# Inclure CPM à partir du chemin où il a été téléchargé
include(${CPM_DOWNLOAD_LOCATION})

# Add external dependencies
CPMAddPackage(
    NAME libconfig
    GIT_REPOSITORY "https://github.com/hyperrealm/libconfig.git"
    GIT_TAG "v1.7.3"
    OPTIONS
    "BUILD_EXAMPLES OFF"
    "BUILD_TESTS OFF"
)

CPMAddPackage(
  NAME Boost
  VERSION 1.86.0
  URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
  OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_INCLUDE_LIBRARIES asio"
)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Define the executable and rename it to "rtype"
add_executable(rtype
    Main.cpp
    AUI.cpp
    Game.cpp
    LevelEditorScene.cpp
    LevelEditorUI.cpp
    MainMenuUI.cpp
    UIButton.cpp
    UIInputField.cpp
    UISlider.cpp
    UISprite.cpp
    Window.cpp
)

# Set the output name for the executable
set_target_properties(rtype PROPERTIES OUTPUT_NAME "rtype")

# Link SFML and other libraries with your executable
target_link_libraries(rtype sfml-graphics sfml-window sfml-system)
