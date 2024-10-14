# Add an executable for the server project
add_library(Scale SHARED
    Scale.cpp
)

target_link_libraries(Scale
    ${LIBCONFIG_LIB_NAME}
)
