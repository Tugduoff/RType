# Add an executable for the server project
add_library(Display SHARED
    Display.cpp
)

target_link_libraries(Display
    sfml-graphics
    sfml-window
    sfml-graphics
    sfml-network
    ${LIBCONFIG_LIB_NAME}
)
