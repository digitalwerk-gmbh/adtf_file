/**
 * @file
 * ADTF Dat Exporter.
 *
 * @copyright
 * @verbatim
   Copyright @ 2020 AUDI AG. All rights reserved.

       This Source Code Form is subject to the terms of the Mozilla
       Public License, v. 2.0. If a copy of the MPL was not distributed
       with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

   If it is not possible or desirable to put the notice in a particular file, then
   You may include the notice in a location (such as a LICENSE file in a
   relevant directory) where a recipient would be likely to look for such a notice.

   You may add additional accurate notices of copyright ownership.
   @endverbatim
 */

#include <adtfdat_processing/reader.h>

namespace adtf
{
namespace dat
{
namespace ant
{

ReaderWrapper::ReaderWrapper(std::shared_ptr<adtf_file::ReaderFactory> factory, const std::string& identifier):
    _factory(factory),
    _identifier(identifier)
{
}

std::string ReaderWrapper::getReaderIdentifier() const
{
    return _identifier;
}

std::pair<bool, std::string> ReaderWrapper::isCompatible(const std::string& url) const
{
    try
    {
        makeReader(url);
        return std::make_pair(true, std::string());
    }
    catch (const std::exception& error)
    {
        return std::make_pair(false, error.what());
    }
}

void ReaderWrapper::open(const std::string& url)
{
    _reader = makeReader(url);
}

std::vector<adtf_file::Stream> ReaderWrapper::getStreams() const
{
    return _reader->getStreams();
}

std::vector<adtf_file::Extension> ReaderWrapper::getExtensions() const
{
    return _reader->getExtensions();
}

adtf_file::FileItem ReaderWrapper::getNextItem()
{
    auto next_item = _reader->getNextItem();
    ++_processed_items;
    return next_item;
}

double ReaderWrapper::getProgress() const
{
    return static_cast<double>(_processed_items) / _reader->getItemCount();
}

std::unique_ptr<adtf_file::BaseReader> ReaderWrapper::makeReader(const std::string& url) const
{
    return _factory->makeReader(url,
                                std::make_shared<adtf_file::sample_factory<adtf_file::DefaultSample>>(),
                                std::make_shared<adtf_file::stream_type_factory<adtf_file::DefaultStreamType>>(),
                                true);
}

ReaderWrapperFactory::ReaderWrapperFactory(std::shared_ptr<adtf_file::ReaderFactory> factory):
      _factory(factory)
{
}

std::shared_ptr<Reader> ReaderWrapperFactory::make() const
{
    return std::make_shared<ReaderWrapper>(_factory, _factory->getName());
}

ReaderFactories getReaderFactories()
{
    ReaderFactories factories;

    for (auto& factory: adtf_file::getObjects().getAllOfType<ReaderFactory>())
    {
        factories.add(factory);
    }

    for (auto& factory: adtf_file::getObjects().getAllOfType<adtf_file::ReaderFactory>())
    {
        factories.add(std::make_shared<ReaderWrapperFactory>(factory));
    }

    return factories;
}

}
}
}
