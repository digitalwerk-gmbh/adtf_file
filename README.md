ADTF File Library      {#mainpage}
=================

## Description

The ADTF File Library will enable you to read and write an .adtfdat file.

* basic serialisations for samples (container for timestamped streamed data)
* basic deserialisations for samples
* the file format IFHD itself (Indexed File Format)

_____________________
 
## ADTF File Library Packages
 
This library contains following internal packages: 
 
### adtf_file (see src/libraries/adtf_file)
 
The ADTF File Package with 'adtf_file::Writer' and 'adtf_file::Reader'. Depending on the ADTF File Version you 
may create and read Indexed Files with samples.
  
### ifhd_file (see src/libraries/ifhd_file)

The IFHD File Package with the 'cIndexedFileWriter' and 'cIndexedFileReader'. Depending on the File Version you 
may create and read Indexed Files. This will not contain sample. The organisation type of data is *chunk*.
 
* 'ifhd::v400' is for ADTF 3.
* 'ifhd::v201_v301' is for ADTF 2.
 
### utils5extension (see src/libraries/utils5extension)

This Package will provide some further helper classes needed to create an Indexed File.

### adtfdat_processing (see src/libraries/adtfdat_processing)

This package will provide interfaces to export (adtf::dat::ant::Processor) and 
import (adtf::dat::ant::Reader) data from/to a adtf_file::Stream of an .adtfdat file.

## ADTF File Library Tools

### adtf_dattool (see src/tools/adtf_dattool)

Command line application to export/import data from/to .adtfdat files by using delivered or 
customized adtf::dat::ant::Processor and adtf::dat::ant::Reader. 
For more information please launch the tool with command --help.

## Further Documentation

See [ADTF File Library Documentation](./doc/input/adtf_file_documentation.md)([link](@ref page_adtf_file_documentation))

## License Information
 
Have also a look at [MPL License](./doc/license/MPL2.0.txt).
and see [License Information](./doc/input/used_licenses.md)([link](@ref page_license))

________________________

## Dependencies

### a_util library
 
The libraries above depend on the *a_util library* 
See http://www.cip.audi.de/bitbucket/projects/OPENDEV/repos/odautil

### DDL library

The libraries above depend on the *DDL library*
See http://www.cip.audi.de/bitbucket/projects/OPENDEV/repos/cdddl
 
### How to build

#### Build Environment
 
The libraries are build and tested only under following compilers and operating systems: 

##### Windows 7 64 Bit

* Visual Studio C++ 2015 Update 3.1 (Update 3 and KB3165756)
 
##### Linux Ubuntu 16.04 LTS

* On other distributions make at least sure that libc has version >= 2.23 and libstdc++ >= 6.0.21.
* gcc 5.4 
 
#### How to build
 
If you can not reach the above online repositories the bundle of it is delivered within a separate download area or installation. 
See [Versions](./doc/input/page_delivered_versions.md)([link](@ref page_delivered_versions)).

##### Build a_util first
 
The ADTF File Library will only build if a installation of a_utils library is provided.
Following libraries of the a_utils are necessary:
* a_util_concurrency
* a_util_memory
* a_util_regex
* a_util_strings
* a_util_datetime
* a_util_filesystem
* a_util_xml
 
- Use CMAKE at least in version 3.5.1.  
- Use the Release Branch \p "Release/a_util-version-branch" see [Versions](./doc/input/page_delivered_versions.md)
- Use CMakeLists.txt within the main directory as Source Directory
- Do not forget to set a CMAKE_INSTALL_PREFIX 
- Build and install for Debug and RelWithDebInfo

##### Build ddl as second step
 
The ADTF File Library will only build if a installation of ddl library is provided.
 
- Use CMAKE at least in version 3.5.1.  
- Use the Release Branch \p "Release/ddl-version-branch". see [Versions](./doc/input/page_delivered_versions.md)
- Use CMakeLists.txt within the main directory as Source Directory
- Do not forget to set a CMAKE_INSTALL_PREFIX 
- Build and install for Debug and RelWithDebInfo
 
##### Build adtf_file
 
- Use CMAKE at least in version 3.5.  
- Use the Release Branch \p "Release/adtf-file-version-branch" See [Versions](./doc/input/page_delivered_versions.md)
- Use CMakeLists.txt within the main directory as Source Directory
- Set the a_util_DIR to your a_util Install "lib/cmake/a_util"
- Do not forget to set a CMAKE_INSTALL_PREFIX 
- Build and install for Debug and RelWithDebInfo
