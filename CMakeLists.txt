# main CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(project VERSION 0.1 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/src)

enable_testing()

add_subdirectory(modules)
add_subdirectory(src)
add_subdirectory(tests EXCLUDE_FROM_ALL)

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
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
