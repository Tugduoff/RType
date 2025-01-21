# Add an executable for the server project
add_library(InputRaylib SHARED
    InputRaylib.cpp

    ../../../src/utils/Actions.cpp
    ../../../src/utils/Keys.cpp
)

target_link_libraries(InputRaylib
    ${LIBCONFIG_LIB_NAME}
)
