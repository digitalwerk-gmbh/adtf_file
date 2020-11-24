add_subdirectory(test_processor)

include(GoogleTest)

enable_testing()

if("${PROJECT_NAME}" STREQUAL "adtffile")
    set(_dattool_exe_path $<TARGET_FILE:adtf_dattool>)
else()
    set(_dattool_exe_path ${adtf_file_DIR}/../../../bin/adtf_dattool$<$<CONFIG:Debug>:d>${CMAKE_EXECUTABLE_SUFFIX})
endif()

add_executable(test_adtf_dattool
    dattool_helper.h
    test_list_streams.cpp
    test_create.cpp
    test_export.cpp
    test_modify_extension.cpp)
target_compile_definitions(test_adtf_dattool PRIVATE
    -DTEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    -DTEST_BUILD_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    -DADTF_DATTOOL_EXECUTABLE="${_dattool_exe_path}"
    -DTEST_PROCESSOR_PLUGIN="$<TARGET_FILE:test_processor>")
target_link_libraries(test_adtf_dattool
                      PRIVATE adtf_file gtest gtest_main $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:stdc++fs>)
target_compile_features(test_adtf_dattool PUBLIC cxx_std_14)

add_test(test_adtf_dattool test_adtf_dattool)
set_tests_properties(${adtf_dattool_tests} PROPERTIES TIMEOUT 20)

