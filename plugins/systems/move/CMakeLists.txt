# Add an executable for the server project
add_library(Move SHARED
    Move.cpp
    ../../../src/Engine/ServerEngine.cpp
    ../../../src/ECS/system/SystemManager.cpp
)

target_link_libraries(Move
    ${LIBCONFIG_LIB_NAME}
)
