# Add an executable for the server project
add_executable(server
    Main.cpp

    ../ECS/system/SystemManager.cpp
    ../GameEngine/GameEngine.cpp
    
    # ../network/UDPServer.cpp
    ../utils/Actions.cpp
    ../utils/Keys.cpp
)

# Set the output name for the server binary
set_target_properties(server PROPERTIES OUTPUT_NAME "r-type_server")

CPMAddPackage(
    NAME SFML 
    GIT_REPOSITORY "https://github.com/SFML/SFML.git"
    GIT_TAG "2.6.1"
    # OPTIONS
    # "BUILD_SHARED_LIBS OFF"
)

target_link_libraries(server
    sfml-graphics
    sfml-window
    sfml-graphics
    sfml-network
    Boost::asio
    raylib
    ${LIBCONFIG_LIB_NAME}
)
