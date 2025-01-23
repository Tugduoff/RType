# Add an executable for the server project
add_library(Rotation SHARED
    Rotation.cpp
)

target_link_libraries(Rotation
    ${LIBCONFIG_LIB_NAME}
)
