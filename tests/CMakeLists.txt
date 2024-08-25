add_custom_target(tests DEPENDS test_scanner test_parser test_dictionary)

add_executable(test_scanner test_scanner.c)
add_executable(test_parser test_parser.c)
add_executable(test_dictionary test_dictionary.c)

target_link_libraries(test_scanner PRIVATE map_parser)
target_link_libraries(test_parser PRIVATE map_parser)
target_link_libraries(test_dictionary PRIVATE dictionary)

add_test(TestParser test_parser)
add_test(TestScanner test_scanner)
add_test(TestDictionary test_dictionary)
