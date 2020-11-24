include(GoogleTest)

enable_testing()

add_executable(test_csv_processor
    test_csv_processor.cpp)
target_compile_definitions(test_csv_processor PRIVATE
    -DTEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    -DTEST_BUILD_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    -DCSV_PROCESSOR_PLUGIN="$<TARGET_FILE:csv_processor>")
target_link_libraries(test_csv_processor PRIVATE adtfdat_processing
                                                 gtest
                                                 gtest_main
                                                 $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:stdc++fs>)

gtest_add_tests(TARGET test_csv_processor
                TEST_LIST   csv_processor_tests)
set_tests_properties(${csv_processor_tests} PROPERTIES TIMEOUT 20)
