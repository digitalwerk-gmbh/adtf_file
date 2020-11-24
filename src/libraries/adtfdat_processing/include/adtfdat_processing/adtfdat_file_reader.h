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

#pragma once

#include "reader.h"
#include <adtf_file/adtf_file_reader.h>

namespace adtf
{
namespace dat
{
namespace ant
{

/**
 * An implementation of a reader that can read ADTFDAT files.
 */
class AdtfDatReader : public ReaderWrapper
{
public:
    AdtfDatReader();
};

}

using ant::AdtfDatReader;
}
}
