# Add an executable for the server project
add_library(RaylibSystem SHARED
    Raylib.cpp
    DisplayManager.cpp
    ModelComponent.cpp
    InputManager.cpp
    RayTexture.cpp

    ../../../src/utils/Keys.cpp
    ../../../src/utils/Actions.cpp
)

CPMAddPackage(
    NAME Raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "5.0"
    OPTIONS
    "BUILD_EXAMPLES OFF"
    "BUILD_GAMES OFF"
    "BUILD_TEST OFF"
    "BUILD_DOCS OFF"
)

target_link_libraries(RaylibSystem
    raylib
    ${LIBCONFIG_LIB_NAME}
)
