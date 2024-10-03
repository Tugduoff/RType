# Add an executable for the client project
add_executable(client
    Main.cpp

    ../ECS/system/SystemManager.cpp
    ../GameEngine/GameEngine.cpp
)

# Set the output name for the server binary
set_target_properties(client PROPERTIES OUTPUT_NAME "r-type_client")

CPMAddPackage(
    NAME SFML 
    GIT_REPOSITORY "https://github.com/SFML/SFML.git"
    GIT_TAG "2.6.1"
    # OPTIONS
    # "BUILD_SHARED_LIBS OFF"
)

target_link_libraries(client
    sfml-graphics
    sfml-window
    sfml-graphics
    sfml-network
    ${LIBCONFIG_LIB_NAME}
)


# if(SFML_ADDED)
#     add_library(SFML INTERFACE IMPORTED)
#     target_include_directories(SFML INTERFACE "${SFML_SOURCE_DIR}/include")
# endif()

# find_library(SFML_SYSTEM sfml-system)
# find_library(SFML_WINDOW sfml-window)
# find_library(SFML_GRAPHICS sfml-graphics)
# find_library(SFML_NETWORK sfml-network)

# target_link_libraries(client
#     ${SFML_SYSTEM}
#     ${SFML_WINDOW}
#     ${SFML_GRAPHICS}
#     ${SFML_NETWORK}
# )
