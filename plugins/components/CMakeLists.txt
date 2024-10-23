# Find all directories containing CMakeLists.txt
file(GLOB_RECURSE SUBDIRS RELATIVE  ${CMAKE_CURRENT_SOURCE_DIR} */CMakeLists.txt)


if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/components)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/components)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release/components)
else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/components)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/components)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release/components)
endif(WIN32)

# Add each subdirectory
foreach(subdir ${SUBDIRS})
    # Get the directory from the file path
    get_filename_component(dir ${subdir} DIRECTORY)
    
    # Add the directory
    add_subdirectory(${dir})
endforeach()
