# Add an executable for the server project
add_library(MoveRaylib SHARED
    MoveRaylib.cpp
)

target_link_libraries(MoveRaylib
    ${LIBCONFIG_LIB_NAME}
)
