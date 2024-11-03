# Add an executable for the server project
add_library(Sound SHARED
    Sound.cpp
)

target_link_libraries(Sound
    ${LIBCONFIG_LIB_NAME}
)
