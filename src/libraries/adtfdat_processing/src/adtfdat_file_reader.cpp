/**
*
* ADTF Dat Exporter.
*
* @file
* Copyright &copy; 2003-2016 Audi Electronics Venture GmbH. All rights reserved
*
* $Author: WROTHFL $
* $Date: 2017-06-06 15:51:27 +0200 (Di, 06 Jun 2017) $
* $Revision: 61312 $
*
* @remarks
*
*/
#include <adtfdat_processing/adtfdat_file_reader.h>
#include <adtf_file/standard_factories.h>

namespace adtf
{
namespace dat
{
namespace ant
{

AdtfDatReader::AdtfDatReader():
      ReaderWrapper(std::make_shared<adtf_file::DefaultAdtfDatReaderFactory>(),
                    "adtfdat")
{
}

}
}
}
