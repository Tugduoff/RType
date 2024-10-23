# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add an executable for the server project
add_library(Config SHARED
    Config.cpp
)

target_link_libraries(Config
    ${LIBCONFIG_LIB_NAME}
)
