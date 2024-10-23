# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add an executable for the server project
add_library(Input SHARED
    Input.cpp
    ../../../Actions.cpp
    ../../../Keys.cpp
)

target_link_libraries(Input
    ${LIBCONFIG_LIB_NAME}
)
