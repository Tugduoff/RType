# Add an executable for the server project
add_library(Sfml SHARED
    Sfml.cpp
    Display.cpp
    Input.cpp
    
    ../../../src/utils/Keys.cpp
    ../../../src/utils/Actions.cpp
    Texture.cpp
)

CPMAddPackage(
    NAME SFML 
    GIT_REPOSITORY "https://github.com/SFML/SFML.git"
    GIT_TAG "2.6.1"
    # OPTIONS
    # "BUILD_SHARED_LIBS OFF"
)

target_link_libraries(Sfml
    sfml-graphics
    sfml-window
    sfml-network
    sfml-system
    sfml-audio
    ${LIBCONFIG_LIB_NAME}
)
