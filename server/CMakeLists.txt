# Add an executable for the server project
add_executable(server Main.cpp)

# Set the output name for the server binary
set_target_properties(server PROPERTIES OUTPUT_NAME "r-type_server")

# Link libraries or add dependencies here, if any
# target_link_libraries(server ...)
