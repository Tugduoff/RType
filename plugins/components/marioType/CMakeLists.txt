# Add an executable for the server project
add_library(MarioType SHARED
    MarioType.cpp
)

target_link_libraries(MarioType
    ${LIBCONFIG_LIB_NAME}
)
