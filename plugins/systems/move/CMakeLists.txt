# Add an executable for the server project
add_library(Move SHARED
    Move.cpp
)

target_link_libraries(Move
    ${LIBCONFIG_LIB_NAME}
)
