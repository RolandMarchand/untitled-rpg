# main CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(project VERSION 0.1 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/src)

enable_testing()

add_subdirectory(modules)
add_subdirectory(tests)
add_subdirectory(src)

message(STATUS "Looking for clang-format")
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  message(STATUS "clang-format found")
  # Used for clang-format
  file(GLOB_RECURSE ALL_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/test/*.c
    ${CMAKE_SOURCE_DIR}/test/*.h
  )
  add_custom_target(format
    COMMAND clang-format -i ${ALL_SOURCE_FILES}
    COMMENT "Formatting source files"
  )
endif()

message(STATUS "Looking for clang-tidy")
find_program(CLANG_TIDY NAMES "clang-tidy")
if (CLANG_TIDY)
  message(STATUS "clang-tidy found")
  set(CMAKE_C_CLANG_TIDY
    clang-tidy
    -p=${CMAKE_BINARY_DIR}/compile_commands.json
    --config-file=${CMAKE_SOURCE_DIR}/src/.clang-tidy
    --verify-config
    --use-color
  )
endif()
