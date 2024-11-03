# Add an executable for the server project
add_library(Ai SHARED
    Ai.cpp
)

target_link_libraries(Ai
    ${LIBCONFIG_LIB_NAME}
)
