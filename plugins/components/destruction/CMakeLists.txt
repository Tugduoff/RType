# Add an executable for the server project
add_library(Destruction SHARED
    Destruction.cpp
)

target_link_libraries(Destruction
    ${LIBCONFIG_LIB_NAME}
)
