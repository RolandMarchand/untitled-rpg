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

  # Use flex to generate scanner for .MAP files
  add_custom_target(generate_scanner
    COMMAND flex -lX --nounistd -o map_scanner.c --header-file=map_scanner.h
    map_scanner.l
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating scanner")
else()
  message(STATUS "Parser generator tools were not found")
endif()

# .MAP file parser
add_library(map_parser
  ${CMAKE_CURRENT_SOURCE_DIR}/map_scanner.c
  ${CMAKE_CURRENT_SOURCE_DIR}/map_parser.c)
