# Add an executable for the client project
add_executable(client Main.cpp)

# Set the output name for the server binary
set_target_properties(client PROPERTIES OUTPUT_NAME "r-type_client")

# Link libraries or add dependencies here, if any
# target_link_libraries(client ...)
