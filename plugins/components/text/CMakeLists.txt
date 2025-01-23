# Add an executable for the server project
add_library(Text SHARED
    Text.cpp
)

target_link_libraries(Text
    ${LIBCONFIG_LIB_NAME}
)
