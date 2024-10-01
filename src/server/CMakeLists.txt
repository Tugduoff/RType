# Add an executable for the server project
add_executable(server
    Main.cpp

    ../DLLoader/DLLoader.cpp
    ../ECS/entity/EntityManager.cpp
    ../ECS/system/SystemManager.cpp
)

# Set the output name for the server binary
set_target_properties(server PROPERTIES OUTPUT_NAME "r-type_server")
