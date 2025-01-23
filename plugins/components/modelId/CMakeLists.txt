# Add an executable for the server project
add_library(ModelId SHARED
    ModelId.cpp
)

target_link_libraries(ModelId
    ${LIBCONFIG_LIB_NAME}
)
