# Add an executable for the server project
add_library(LeafBlowerSpawnSystem SHARED
    Spawn.cpp
)

target_link_libraries(LeafBlowerSpawnSystem
    ${LIBCONFIG_LIB_NAME}
)
