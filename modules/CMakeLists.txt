add_subdirectory(raylib)

add_library(whereami STATIC whereami/src/whereami.c)
target_include_directories(whereami PUBLIC whereami/src)
