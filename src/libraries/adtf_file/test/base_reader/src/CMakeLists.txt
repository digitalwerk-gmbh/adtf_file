set(TEST t_base_reader) #to not exceed 260 chars on path under windows...

set(TEST_FILES_DIR "${CMAKE_CURRENT_LIST_DIR}/../files")

add_executable(${TEST} tester_base_reader.cpp)
target_link_libraries(${TEST} PRIVATE gtest gtest_main adtf_file)
target_compile_definitions(${TEST} PRIVATE TEST_FILES_DIR="${TEST_FILES_DIR}")
add_test(${TEST} ${TEST})
set_target_properties(${TEST} PROPERTIES FOLDER test/adtf_file)
