# Add an executable for the server project
add_library(Position3d SHARED
    Position3d.cpp
)

target_link_libraries(Position3d
    ${LIBCONFIG_LIB_NAME}
)
