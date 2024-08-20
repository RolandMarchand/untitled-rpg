add_custom_target(tests DEPENDS test_scanner test_parser)

# Scanner test
add_executable(test_scanner test_scanner.c)
target_link_libraries(test_scanner PRIVATE map_parser)
add_test(TestScanner test_scanner)

# Parser test
add_executable(test_parser test_parser.c)
target_link_libraries(test_parser PRIVATE map_parser)
add_test(TestParser test_parser)
