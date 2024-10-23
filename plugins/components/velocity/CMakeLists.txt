# Add an executable for the server project
add_library(Velocity SHARED
    Velocity.cpp
)

target_link_libraries(Velocity
    ${LIBCONFIG_LIB_NAME}
)
