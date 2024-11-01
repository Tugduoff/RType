# Add an executable for the server project
add_library(Velocity3d SHARED
    Velocity3d.cpp
)

target_link_libraries(Velocity3d
    ${LIBCONFIG_LIB_NAME}
)
