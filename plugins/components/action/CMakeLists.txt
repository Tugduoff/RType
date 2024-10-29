# Add an executable for the server project
add_library(Action SHARED
    Action.cpp
)

target_link_libraries(Action
    ${LIBCONFIG_LIB_NAME}
)
