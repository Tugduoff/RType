# Add an executable for the server project
add_library(Controllable SHARED
    Controllable.cpp
)

target_link_libraries(Controllable
    ${LIBCONFIG_LIB_NAME}
)
