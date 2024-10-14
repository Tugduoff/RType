# Add an executable for the server project
add_library(Collision SHARED
    Collision.cpp
)

target_link_libraries(Collision
    ${LIBCONFIG_LIB_NAME}
)
