##get all tests listed inside file 'ifhd/ifhd_test_projects.cmake' (as cmake subdirectories)
find_package(ifhd_file REQUIRED)
include(../../src/libraries/ifhd_file/ifhd_test_projects.cmake)

find_package(adtf_file REQUIRED)
include(../../src/libraries/adtf_file/adtf_file_test_projects.cmake)

find_package(adtfdat_processing REQUIRED)
include(../../src/tools/adtf_dattool/adtf_dattool_test_projects.cmake)

find_package(adtfdat_processing REQUIRED)
include(../../src/libraries/adtfdat_processing/adtfdat_processing_test_projects.cmake)
