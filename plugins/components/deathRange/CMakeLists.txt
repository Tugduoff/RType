# Add an executable for the server project
add_library(DeathRange SHARED
    DeathRange.cpp
)

target_link_libraries(DeathRange
    ${LIBCONFIG_LIB_NAME}
)
