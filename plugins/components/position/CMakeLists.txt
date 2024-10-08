# Add an executable for the server project
add_library(Position SHARED
    Position.cpp
)

target_link_libraries(Position
    ${LIBCONFIG_LIB_NAME}
)
