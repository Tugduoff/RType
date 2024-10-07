# Add an executable for the server project
add_library(Position SHARED
    Position.cpp
    ../../../src/Engine/ServerEngine.cpp
    ../../../src/ECS/system/SystemManager.cpp
)

target_link_libraries(Position
    ${LIBCONFIG_LIB_NAME}
)
