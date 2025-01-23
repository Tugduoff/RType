# Add an executable for the server project
add_library(Score SHARED
    Score.cpp
)

target_link_libraries(Score
    ${LIBCONFIG_LIB_NAME}
)
