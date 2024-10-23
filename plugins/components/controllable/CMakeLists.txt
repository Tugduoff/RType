# Add an executable for the server project
add_library(Controllable SHARED
    Controllable.cpp
    ../../../src/utils/Keys.cpp
    ../../../src/utils/Actions.cpp
)

target_link_libraries(Controllable
    ${LIBCONFIG_LIB_NAME}
)
