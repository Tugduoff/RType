# Add an executable for the server project
add_executable(server
    Main.cpp

    ../ECS/system/SystemManager.cpp
    ../GameEngine/GameEngine.cpp
    ../../plugins/components/controllable/Actions.cpp
    ../../plugins/components/controllable/Keys.cpp
)

# Set the output name for the server binary
set_target_properties(server PROPERTIES OUTPUT_NAME "r-type_server")

target_link_libraries(server
    ${LIBCONFIG_LIB_NAME}
)
