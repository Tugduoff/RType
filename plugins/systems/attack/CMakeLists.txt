# Add an executable for the server project
add_library(Attack SHARED
    Attack.cpp
)

target_link_libraries(Attack
    ${LIBCONFIG_LIB_NAME}
)
