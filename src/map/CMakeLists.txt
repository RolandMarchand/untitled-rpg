message(STATUS "Looking for Bison")
find_program(BISON_FOUND NAMES bison)

message(STATUS "Looking for Flex")
find_program(FLEX_FOUND NAMES flex)

if (BISON_FOUND AND FLEX_FOUND)
  message(STATUS "Parser generator tools found")

  add_custom_target(generate_parser DEPENDS generate_parser_proper generate_scanner)

  # First, generate the bison parser for .MAP files
  add_custom_target(generate_parser_proper
    COMMAND bison -Wall -Hmap_parser.h -omap_parser.c map_parser.y
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating parser")

  if (WIN32)
    set (ADDITIONAL_FLEX_FLAGS "--wincompat")
  else()
    set (ADDITIONAL_FLEX_FLAGS "")
  endif()

  # Use flex to generate scanner for .MAP files
  add_custom_target(generate_scanner
    COMMAND flex ${ADDITIONAL_FLEX_FLAGS} --nounistd -o map_scanner.c --header-file=map_scanner.h
    map_scanner.l
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating scanner")
else()
  message(STATUS "Parser generator tools were not found")
endif()

# .MAP file parser
add_library(map map.c map_scanner.c map_parser.c)
target_link_libraries(map dictionary)
set_source_files_properties(map_parser.c PROPERTIES
  SKIP_LINTING ON)
set_source_files_properties(map_scanner.c PROPERTIES
  SKIP_LINTING ON)
set_source_files_properties(map_parser.h PROPERTIES
  SKIP_LINTING ON)
set_source_files_properties(map_scanner.h PROPERTIES
  SKIP_LINTING ON)

