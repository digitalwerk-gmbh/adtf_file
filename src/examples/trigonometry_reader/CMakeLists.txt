cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
project(trigonometry_reader)

if (NOT TARGET adtf_file)
    find_package(adtf_file PATHS ${CMAKE_SOURCE_DIR}/lib/cmake/adtf_file REQUIRED)
endif()

add_library(trigonometry_reader MODULE
    trigonomentry_reader.h
    trigonomentry_reader.cpp
    plugin.cpp
)

target_link_libraries(trigonometry_reader adtf_file)

set_target_properties(trigonometry_reader PROPERTIES
    PREFIX ""
    SUFFIX ".adtffileplugin"
    DEFINE_SYMBOL ""
    DEBUG_POSTFIX "d"
    FOLDER examples
)

install(TARGETS trigonometry_reader
    DESTINATION bin
)

if(ifhd_cmake_enable_integrated_tests)
    add_subdirectory(test)
endif()
