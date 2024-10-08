# Add an executable for the server project
add_library(Visible SHARED
    Visible.cpp
)

target_link_libraries(Visible
    ${LIBCONFIG_LIB_NAME}
)
