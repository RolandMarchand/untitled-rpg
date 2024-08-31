add_custom_target(tests DEPENDS
  test_scanner test_parser test_dictionary test_obj)

add_executable(test_dictionary EXCLUDE_FROM_ALL test_dictionary.c)
add_executable(test_obj EXCLUDE_FROM_ALL test_obj.c)
add_executable(test_parser EXCLUDE_FROM_ALL test_parser.c)
add_executable(test_scanner EXCLUDE_FROM_ALL test_scanner.c)

target_link_libraries(test_dictionary PRIVATE dictionary)
target_link_libraries(test_obj PRIVATE map)
target_link_libraries(test_parser PRIVATE map)
target_link_libraries(test_scanner PRIVATE map)

add_test(TestDictionary test_dictionary)
add_test(TestObj test_obj)
add_test(TestParser test_parser)
add_test(TestScanner test_scanner)
