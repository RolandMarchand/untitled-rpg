# main CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(project VERSION 0.1 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(modules)
add_subdirectory(src)
