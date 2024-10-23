# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add an executable for the server project
add_library(Collision SHARED
    Collision.cpp
)

target_link_libraries(Collision
    ${LIBCONFIG_LIB_NAME}
)
