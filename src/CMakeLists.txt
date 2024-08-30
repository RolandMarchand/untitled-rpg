add_subdirectory(map)

message(STATUS "Looking for clang-tidy")
find_program(CLANG_TIDY NAMES "clang-tidy")

if (CLANG_TIDY)
  message(STATUS "clang-tidy found")
  set(CMAKE_C_CLANG_TIDY clang-tidy -p ${CMAKE_BUILD_DIR} ./src/)
endif()

add_library(dictionary dictionary.c)
if (MSVC)
	target_compile_options(dictionary PRIVATE /W4)
else()
	target_compile_options(dictionary PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Main executable
add_executable(main main.c resources.c)
add_dependencies(main archive_resources)
target_link_libraries(main PRIVATE raylib dictionary whereami map)
set_target_properties(main
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
if (MSVC)
	target_compile_options(main PRIVATE /W4)
else()
	target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Package the assets folder into a resource package for the game to access files
add_custom_target(archive_resources
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets
  ${CMAKE_BINARY_DIR}/resources)
# Make the resource package cleanable
set_target_properties(archive_resources
  PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_BINARY_DIR}/resources)
