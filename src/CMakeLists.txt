add_subdirectory(map)

add_library(dictionary dictionary.c)
target_compile_options(dictionary PRIVATE -Wall -Wextra -Wpedantic)

# Main executable
add_executable(main main.c resources.c)
add_dependencies(main archive_resources)
target_link_libraries(main PRIVATE raylib dictionary whereami)
set_target_properties(main
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_compile_options(main PRIVATE -Wall -Wextra) # Enable warnings

# Package the assets folder into a resource package for the game to access files
add_custom_target(archive_resources
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets
  ${CMAKE_BINARY_DIR}/resources)
# Make the resource package cleanable
set_target_properties(archive_resources
  PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_BINARY_DIR}/resources)
