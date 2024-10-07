# Add an executable for the server project
add_library(Config SHARED
    Config.cpp
    ../../../src/Engine/ServerEngine.cpp
    ../../../src/ECS/system/SystemManager.cpp
)

target_link_libraries(Config
    ${LIBCONFIG_LIB_NAME}
)
