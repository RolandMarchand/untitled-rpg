set(RESOURCE ${CMAKE_BINARY_DIR}/resource.zip)

add_library(resource resource.c)
target_link_libraries(resource PRIVATE miniz)

add_executable(main main.c)
target_link_libraries(main PRIVATE raylib resource)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# Package the assets folder into a resource package for the game to access files
add_custom_target(archive_resources
  # Error 12 means the resource is up to date, ignore
  COMMAND zip -0ru ${RESOURCE} assets > /dev/null || true
  COMMAND test -e ${RESOURCE} || (echo "Error: failed to package resources into resource.zip" && false)
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Packaging resources"
)
# Make the resource package cleanable
set_target_properties(archive_resources PROPERTIES ADDITIONAL_CLEAN_FILES ${RESOURCE})
add_dependencies(main archive_resources)
