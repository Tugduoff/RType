# Add an executable for the server project
add_library(Sfml SHARED
    Sfml.cpp
    Display.cpp
    Input.cpp
)

target_link_libraries(Sfml
    sfml-graphics
    sfml-window
    sfml-graphics
    sfml-network
    ${LIBCONFIG_LIB_NAME}
)
