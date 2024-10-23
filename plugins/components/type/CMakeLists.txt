# Add an executable for the server project
add_library(Type SHARED
    Type.cpp
)

target_link_libraries(Type
    ${LIBCONFIG_LIB_NAME}
)
