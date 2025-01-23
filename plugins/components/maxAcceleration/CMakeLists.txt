# Add an executable for the server project
add_library(MaxAcceleration SHARED
    MaxAcceleration.cpp
)

target_link_libraries(MaxAcceleration
    ${LIBCONFIG_LIB_NAME}
)
