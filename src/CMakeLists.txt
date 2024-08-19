set(RESOURCE ${CMAKE_BINARY_DIR}/resource.zip)

# Game asset management library
add_library(resource resource.c)
target_link_libraries(resource PRIVATE miniz)
target_compile_options(resource PRIVATE -Wall -Wextra)

# Use flex to generate scanner
add_custom_target(generate_scanner
  COMMAND flex -o ${CMAKE_BINARY_DIR}/src/map_scanner.c
  --header-file=${CMAKE_BINARY_DIR}/src/map_scanner.h
  ${CMAKE_SOURCE_DIR}/src/map_scanner.l
  COMMENT "Generating scanner"
)
set_target_properties(generate_scanner PROPERTIES ADDITIONAL_CLEAN_FILES
  "${CMAKE_BINARY_DIR}/src/map_scanner.c;${CMAKE_BINARY_DIR}/src/map_scanner.h"
)
# Run when generating build files.
execute_process(
  COMMAND flex -o ${CMAKE_BINARY_DIR}/src/map_scanner.c
  --header-file=${CMAKE_BINARY_DIR}/src/map_scanner.h
  ${CMAKE_SOURCE_DIR}/src/map_scanner.l
  COMMAND_ECHO STDOUT
)

# .MAP file parser
add_library(map_parser ${CMAKE_BINARY_DIR}/src/map_scanner.c)
add_dependencies(map_parser generate_scanner)

# Main executable
add_executable(main main.c)
target_link_libraries(main PRIVATE raylib resource)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
# Enable warnings
target_compile_options(main PRIVATE -Wall -Wextra)

# Package the assets folder into a resource package for the game to access files
add_custom_target(archive_resources
  # Error 12 means the resource is up to date, ignore
  COMMAND zip -9ru ${RESOURCE} * > /dev/null || true
  COMMAND test -e ${RESOURCE} || (echo "Error: failed to package resources into resource.zip" && false)
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/assets
  COMMENT "Packaging resources"
)
# Make the resource package cleanable
set_target_properties(archive_resources PROPERTIES ADDITIONAL_CLEAN_FILES ${RESOURCE})
add_dependencies(main archive_resources)

