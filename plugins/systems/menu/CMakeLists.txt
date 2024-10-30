# Add an executable for the server project
add_library(Menu SHARED
    Menu.cpp
)

target_link_libraries(Menu
    ${LIBCONFIG_LIB_NAME}
)
