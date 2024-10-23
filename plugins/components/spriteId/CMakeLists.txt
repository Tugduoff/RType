# Add an executable for the server project
add_library(SpriteID SHARED
    SpriteID.cpp
)

target_link_libraries(SpriteID
    ${LIBCONFIG_LIB_NAME}
)
