# Add an executable for the server project
add_library(Raylib SHARED
    Raylib.cpp
    DisplayManager.cpp
    ModelComponent.cpp
    InputManager.cpp

    ../../../src/utils/Keys.cpp
    ../../../src/utils/Actions.cpp
)

target_link_libraries(Raylib
    raylib
    ${LIBCONFIG_LIB_NAME}
)
