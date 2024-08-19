add_custom_target(tests DEPENDS test_scanner)

# Scanner test
add_executable(test_scanner test_scanner.c)
target_link_libraries(test_scanner PRIVATE map_parser)
add_test(TestScanner test_scanner)
