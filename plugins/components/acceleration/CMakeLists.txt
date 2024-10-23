# Add an executable for the server project
add_library(Acceleration SHARED
    Acceleration.cpp
)

target_link_libraries(Acceleration
    ${LIBCONFIG_LIB_NAME}
)
