enable_testing()

set(_current_dir ${CMAKE_CURRENT_LIST_DIR})

##Tests are included from both the integrated build as well as the external tester build. CMake
# requires an explicit binary dir if the folder is not a direct sub-directory, hence this wrapper
function(add_test_subdirectory SUBDIR)
    ##don't use googletest TEST macro, instead use util's own TEST_REQ()
    # maybe redefined in subdir if needed
    add_definitions(-DGTEST_DONT_DEFINE_TEST)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${SUBDIR} ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR})
    message(STATUS "---- ${CMAKE_CURRENT_LIST_DIR}/${SUBDIR}")
endfunction(add_test_subdirectory SUBDIR)

message(STATUS "-- Found following tests for package 'adtf_dattool':")

add_test_subdirectory(test/dattool)

unset(_current_dir)
