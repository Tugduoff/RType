# Add an executable for the server project
add_library(Health SHARED
    Health.cpp
)

target_link_libraries(Position
    ${LIBCONFIG_LIB_NAME}
)
