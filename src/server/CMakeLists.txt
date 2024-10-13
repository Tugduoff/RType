# Add an executable for the server project
add_executable(server
    Main.cpp

    ../ECS/system/SystemManager.cpp
    ../GameEngine/GameEngine.cpp
    ../../Actions.cpp
    ../../Keys.cpp
)

# Set the output name for the server binary
set_target_properties(server PROPERTIES OUTPUT_NAME "r-type_server")

target_link_libraries(server
    sfml-graphics
    sfml-window
    sfml-graphics
    sfml-network
    ${LIBCONFIG_LIB_NAME}
)
