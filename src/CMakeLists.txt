set(RESOURCE ${CMAKE_BINARY_DIR}/resource.zip)

# Game asset management library
add_library(resource resource.c)
target_link_libraries(resource PRIVATE whereami)
target_compile_options(resource PRIVATE -Wall -Wextra -Wpedantic)

add_library(dictionary dictionary.c)
target_compile_options(dictionary PRIVATE -Wall -Wextra -Wpedantic)

# First, generate the bison parser for .MAP files
add_custom_target(generate_parser
  ALL
  COMMAND bison -Wall -H${CMAKE_BINARY_DIR}/src/map_parser.h
  -o${CMAKE_BINARY_DIR}/src/map_parser.c ${CMAKE_SOURCE_DIR}/src/map_parser.y
  COMMENT "Generating parser"
)
# Mark the generated parser as cleanable
set_target_properties(generate_parser PROPERTIES ADDITIONAL_CLEAN_FILES
  "${CMAKE_BINARY_DIR}/src/map_parser.c;${CMAKE_BINARY_DIR}/src/map_parser.h"
)
# Run when generating build files
execute_process(
  COMMAND bison -Wall -H${CMAKE_BINARY_DIR}/src/map_parser.h
  -o${CMAKE_BINARY_DIR}/src/map_parser.c ${CMAKE_SOURCE_DIR}/src/map_parser.y
  COMMAND_ECHO STDOUT
)

# Use flex to generate scanner for .MAP files
add_custom_target(
  generate_scanner
  ALL
  COMMAND flex -o ${CMAKE_BINARY_DIR}/src/map_scanner.c
  --header-file=${CMAKE_BINARY_DIR}/src/map_scanner.h
  ${CMAKE_SOURCE_DIR}/src/map_scanner.l
  COMMENT "Generating scanner"
)
# Mark the generated scanner as cleanable
set_target_properties(
  generate_scanner PROPERTIES ADDITIONAL_CLEAN_FILES
  "${CMAKE_BINARY_DIR}/src/map_scanner.c;${CMAKE_BINARY_DIR}/src/map_scanner.h"
)
# Run when generating build files
execute_process(
  COMMAND flex -o ${CMAKE_BINARY_DIR}/src/map_scanner.c
  --header-file=${CMAKE_BINARY_DIR}/src/map_scanner.h
  ${CMAKE_SOURCE_DIR}/src/map_scanner.l
  COMMAND_ECHO STDOUT
)

# .MAP file parser
add_library(
  map_parser
  ${CMAKE_BINARY_DIR}/src/map_scanner.c
  ${CMAKE_BINARY_DIR}/src/map_parser.c
)
add_dependencies(map_parser generate_scanner generate_parser)

# Main executable
add_executable(main main.c)
target_link_libraries(main PRIVATE raylib resource)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
# Enable warnings
target_compile_options(main PRIVATE -Wall -Wextra)

# Package the assets folder into a resource package for the game to access files
add_custom_target(archive_resources
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/resources
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
# Make the resource package cleanable
set_target_properties(archive_resources PROPERTIES ADDITIONAL_CLEAN_FILES ${RESOURCE})
add_dependencies(main archive_resources)
