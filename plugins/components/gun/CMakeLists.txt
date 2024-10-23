# Add an executable for the server project
add_library(Gun SHARED
    Gun.cpp
)

target_link_libraries(Gun
    ${LIBCONFIG_LIB_NAME}
)
