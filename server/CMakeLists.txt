# Add an executable for the server project
add_executable(server Main.cpp)

# Set the output name for the server binary
set_target_properties(server PROPERTIES OUTPUT_NAME "r-type_server")

# Find the SFML package (make sure SFML is installed)
# find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_library(SFML_SYSTEM sfml-system)
find_library(SFML_WINDOW sfml-window)
find_library(SFML_GRAPHICS sfml-graphics)
find_library(SFML_NETWORK sfml-network)

target_link_libraries(server
    ${SFML_SYSTEM}
    ${SFML_WINDOW}
    ${SFML_GRAPHICS}
    ${SFML_NETWORK}
)
