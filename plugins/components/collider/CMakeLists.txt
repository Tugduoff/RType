# Add an executable for the server project
add_library(Collider SHARED
    Collider.cpp
)

target_link_libraries(Collider
    ${LIBCONFIG_LIB_NAME}
)
