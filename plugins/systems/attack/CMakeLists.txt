# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add an executable for the server project
add_library(Attack SHARED
    Attack.cpp
)

target_link_libraries(Attack
    ${LIBCONFIG_LIB_NAME}
)
