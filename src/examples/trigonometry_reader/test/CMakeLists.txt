include(GoogleTest)

enable_testing()

add_executable(test_trigonometry_reader
    test_trigonometry_reader.cpp)
target_compile_definitions(test_trigonometry_reader PRIVATE
    -DTEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    -DTEST_BUILD_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    -DTRIGONOMETRY_READER_PLUGIN="$<TARGET_FILE:trigonometry_reader>")
target_link_libraries(test_trigonometry_reader PRIVATE adtf_file
                                              gtest
                                              gtest_main)

gtest_add_tests(TARGET test_trigonometry_reader
                TEST_LIST   trigonometry_reader_tests)
set_tests_properties(${trigonometry_reader_tests} PROPERTIES TIMEOUT 20)
