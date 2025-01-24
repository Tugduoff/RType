# Add an executable for the server project
add_library(Collision3d SHARED
    Collision3d.cpp
)

target_link_libraries(Collision3d
    ${LIBCONFIG_LIB_NAME}
)
