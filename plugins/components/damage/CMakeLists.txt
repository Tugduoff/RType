# Add an executable for the server project
add_library(Damage SHARED
    Damage.cpp
)

target_link_libraries(Damage
    ${LIBCONFIG_LIB_NAME}
)
