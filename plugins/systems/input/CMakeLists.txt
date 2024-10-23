# Add an executable for the server project
add_library(Input SHARED
    Input.cpp
    ../../../src/utils/Actions.cpp
    ../../../src/utils/Keys.cpp
)

target_link_libraries(Input
    ${LIBCONFIG_LIB_NAME}
)
