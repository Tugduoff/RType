# Add an executable for the client project
add_executable(client Main.cpp)

# Set the output name for the server binary
set_target_properties(client PROPERTIES OUTPUT_NAME "r-type_client")



find_library(SFML_SYSTEM sfml-system)
find_library(SFML_WINDOW sfml-window)
find_library(SFML_GRAPHICS sfml-graphics)
find_library(SFML_NETWORK sfml-network)

target_link_libraries(client
    ${SFML_SYSTEM}
    ${SFML_WINDOW}
    ${SFML_GRAPHICS}
    ${SFML_NETWORK}
)

# Link libraries or add dependencies here, if any
# target_link_libraries(client ...)
# add dependencies
# include(../cmake/CPM.cmake)

# Initialize variables for distro detection
# set(DISTRO "Unknown")

# # Check if /etc/os-release exists
# if(EXISTS "/etc/os-release")
#     file(READ "/etc/os-release" OS_RELEASE_CONTENT)

#     if(OS_RELEASE_CONTENT MATCHES "ID=debian")
#         set(DISTRO "Debian")
#     elseif(OS_RELEASE_CONTENT MATCHES "ID=ubuntu")
#         set(DISTRO "Ubuntu")
#     elseif(OS_RELEASE_CONTENT MATCHES "ID=fedora")
#         set(DISTRO "Fedora")
#     endif()
# endif()

# message(STATUS "Detected Linux distribution: ${DISTRO}")

# if(${DISTRO} STREQUAL "Ubuntu")
#     message(STATUS "Performing Ubuntu-specific tasks")
#     add_custom_target(run_bash_script ALL
#         COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/apt_sfml_dep.sh
#     )
# elseif(${DISTRO} STREQUAL "Debian")
#     message(STATUS "Performing Debian-specific tasks")
#     add_custom_target(run_bash_script ALL
#         COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/apt_sfml_dep.sh
#     )
# elseif(${DISTRO} STREQUAL "Fedora")
#     message(STATUS "Performing Fedora-specific tasks")
#     add_custom_target(run_bash_script ALL
#         COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/dnf_sfml_dep.sh
#     )
# endif()


# CPMAddPackage(
#     NAME Python 
#     GIT_REPOSITORY "https://github.com/python/cpython.git"
#     GIT_TAG "v3.11.10"
# )

# CPMAddPackage(
#     NAME Flex
#     GIT_REPOSITORY "https://github.com/westes/flex.git"
#     GIT_TAG "v2.6.4"
# )

# CPMAddPackage(
#     NAME Bison
#     GIT_REPOSITORY "https://github.com/akimd/bison.git"
#     GIT_TAG "v3.8.2"
# )

# CPMAddPackage(
#     NAME Doxygen 
#     GIT_REPOSITORY "https://github.com/doxygen/doxygen.git"
#     GIT_TAG "Release_1_12_0"
# )

# CPMAddPackage(
#     NAME flac 
#     GIT_REPOSITORY "https://github.com/xiph/flac.git"
#     GIT_TAG "1.4.3"
#     # OPTIONS
#     # "INSTALL_MANPAGES OFF"
# )

# CPMAddPackage(
#     NAME SFML 
#     GIT_REPOSITORY "https://github.com/SFML/SFML.git"
#     GIT_TAG "2.6.1"
# )

# CPMAddPackage("gh:SFML/SFML#2.6.1")

# Find the SFML package (make sure SFML is installed)
# find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
