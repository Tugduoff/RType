# Add an executable for the server project
add_library(Behavior SHARED
    Behavior.cpp
)

target_link_libraries(Behavior
    ${LIBCONFIG_LIB_NAME}
)