# Add an executable for the server project
add_library(LeafBlowerMoveSystem SHARED
    Move.cpp
)

include_directories(./build/_deps/raylib-src/src)
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

# set(RAYLIB_LIBRARY_NAME "libraylib.a")
# link_directories(./build/_deps/raylib-build/raylib/)
include_directories(./build/_deps/raylib-build/raylib/include)
cmake_policy(SET CMP0079 NEW)

target_link_libraries(LeafBlowerMoveSystem
    raylib
    ${LIBCONFIG_LIB_NAME}
)
