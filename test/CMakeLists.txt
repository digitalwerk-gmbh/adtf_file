cmake_minimum_required(VERSION 3.0.0)   #minimum for generated find scripts

project(ifhd_tests)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14) # std::make_unique
set(CMAKE_CXX_EXTENSIONS OFF)

option(ifhd_cmake_enable_integrated_tests "Enable tests as integrated build (requires googletest)" ON)

# Use conan if available
if(CONAN_COMPILER)
    if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
        include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo.cmake)
        include(${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo.cmake)
    elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
        include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
    elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo_multi.cmake)
        include(${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo_multi.cmake)
    else()
        message(FATAL_ERROR "Conan build info can't be found.")
    endif()

    if(CORTEX_WORKSPACE)
        conan_basic_setup(TARGETS)
    else()
        conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
    endif()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

##single point of include, the same as for project 'a_util'
include_directories(standalone)

##googletest integration
add_subdirectory(../3rdparty/googletest ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/googletest)

##functional test integration
add_subdirectory(function)
