# Add an executable for the server project
add_library(Controllable SHARED
    Controllable.cpp
    ../../systems/input/Keys.cpp
    ../../systems/input/Actions.cpp
)

target_link_libraries(Controllable
    ${LIBCONFIG_LIB_NAME}
)
