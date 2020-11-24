/**
 * @file
 * adtf file reader.
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

#ifndef ADTF_FILE_BASE_READER_INCLUDED
#define ADTF_FILE_BASE_READER_INCLUDED

#include <chrono>
#include <memory>
#include <vector>
#include <unordered_map>
#include <istream>
#include <string>
#include <ifhd/ifhd.h>
#include "stream_item.h"
#include "object.h"
#include "default_sample.h"
#include "stream_type.h"

namespace adtf_file
{

namespace exceptions
{
    using ifhd::exceptions::EndOfFile;
}

class Extension
{
public:
    std::string name;
    uint16_t stream_id;
    uint32_t user_id;
    uint32_t type_id;
    uint32_t version_id;
    size_t data_size;
    const void* data;
};

class StreamTypeFactory
{
    public:
        virtual std::shared_ptr<StreamType> build() const = 0;
};

template <typename STREAM_TYPE_CLASS>
class stream_type_factory: public StreamTypeFactory
{
    public:
        std::shared_ptr<StreamType> build() const override
        {
            return std::make_shared<STREAM_TYPE_CLASS>();
        }
};

class SampleFactory
{
    public:
        virtual std::shared_ptr<Sample> build() const = 0;
};

template <typename SAMPLE_CLASS>
class sample_factory: public SampleFactory
{
    public:
        std::shared_ptr<Sample> build() const override
        {
            return std::make_shared<SAMPLE_CLASS>();
        }
};

class Stream
{
public:
    uint16_t stream_id;
    std::string name;
    uint64_t item_count;
    std::chrono::nanoseconds timestamp_of_first_item;
    std::chrono::nanoseconds timestamp_of_last_item;
    std::shared_ptr<const StreamType> initial_type;
};

class FileItem
{
public:
    uint16_t stream_id;
    std::chrono::nanoseconds time_stamp;
    std::shared_ptr<const StreamItem> stream_item;
};

class BaseReader
{
public:
    virtual ~BaseReader() = default;

    virtual uint32_t getFileVersion() const = 0;
    virtual a_util::datetime::DateTime getDateTime() const = 0;
    virtual std::chrono::nanoseconds getFirstTime() const = 0;
    virtual std::chrono::nanoseconds getLastTime() const = 0;
    virtual std::chrono::nanoseconds getDuration() const = 0;
    virtual std::string getDescription() const = 0;

    virtual const std::vector<Extension>& getExtensions() const = 0;
    virtual const std::vector<Stream>& getStreams() const = 0;
    virtual uint64_t getItemCount() const = 0;

    virtual uint64_t getItemIndexForTimeStamp(std::chrono::nanoseconds time_stamp) = 0;
    virtual uint64_t getItemIndexForStreamItemIndex(uint16_t stream_id, uint64_t stream_item_index) = 0;
    virtual std::shared_ptr<const StreamType> getStreamTypeBefore(uint64_t item_index, uint16_t stream_id, bool update_sample_deserializer) = 0;

    virtual void seekTo(uint64_t item_index) = 0;
    virtual FileItem getNextItem() = 0;
    virtual int64_t getNextItemIndex() = 0;

};

class ReaderFactory: public Object
{
public:
    virtual std::string getName() const = 0;
    virtual std::unique_ptr<BaseReader> makeReader(const std::string& file_name,                                                   
                                                   std::shared_ptr<SampleFactory> sample_factory,
                                                   std::shared_ptr<StreamTypeFactory> stream_type_factory,
                                                   bool ignore_unsupported_streams) const = 0;
};

class ReaderFactories
{
public:
    void add(const std::shared_ptr<const ReaderFactory>& factory);
    std::unique_ptr<BaseReader> makeReader(const std::string& file_name,                                           
                                           std::shared_ptr<SampleFactory> sample_factory = std::make_shared<adtf_file::sample_factory<DefaultSample>>(),
                                           std::shared_ptr<StreamTypeFactory> stream_type_factory = std::make_shared<adtf_file::stream_type_factory<DefaultStreamType>>(),
                                           bool ignore_unsupported_streams = false) const;

private:
    std::vector<std::shared_ptr<const ReaderFactory>> _factories;
};

}

#endif // ADTF_FILE_BASE_READER_INCLUDED
