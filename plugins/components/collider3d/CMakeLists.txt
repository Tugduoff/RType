# Add an executable for the server project
add_library(Collider3d SHARED
    Collider3d.cpp
)

target_link_libraries(Collider3d
    ${LIBCONFIG_LIB_NAME}
)
