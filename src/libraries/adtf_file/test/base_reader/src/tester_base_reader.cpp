/**
 * @file
 * Tester init.
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

#include "gtest/gtest.h"
#include <adtf_file/base_reader.h>
#include <adtf_file/standard_factories.h>
#include <ddl.h>

using namespace adtf_file;

class TestReader: public BaseReader
{
public:
    TestReader(const std::string& file)
    {
    }

    uint32_t getFileVersion() const override
    {
        return 0;
    }

    a_util::datetime::DateTime getDateTime() const override
    {
        return a_util::datetime::DateTime();
    }

    std::chrono::nanoseconds getFirstTime() const override
    {
        return std::chrono::seconds(0);
    }

    std::chrono::nanoseconds getLastTime() const override
    {
        return std::chrono::seconds(0);
    }

    std::chrono::nanoseconds getDuration() const override
    {
        return std::chrono::seconds(0);
    }

    std::string getDescription() const override
    {
        return "description";
    }

    const std::vector<Extension>& getExtensions() const override
    {
        return _extensions;
    }

    const std::vector<Stream>& getStreams() const override
    {
        return _streams;
    }

    uint64_t getItemCount() const override
    {
        return 0;
    }

    uint64_t getItemIndexForTimeStamp(std::chrono::nanoseconds time_stamp) override
    {
        return 0;
    }

    uint64_t getItemIndexForStreamItemIndex(uint16_t stream_id, uint64_t stream_item_index) override
    {
        return 0;
    }

    std::shared_ptr<const StreamType> getStreamTypeBefore(uint64_t item_index, uint16_t stream_id, bool update_sample_deserializer) override
    {
        return nullptr;
    }

    void seekTo(uint64_t item_index) override
    {
    }

    FileItem getNextItem() override
    {
        throw adtf_file::exceptions::EndOfFile();
    }

    int64_t getNextItemIndex() override
    {
        return -1;
    }

private:
    std::vector<Extension> _extensions;
    std::vector<Stream> _streams;
};

class TestReaderFactory: public ReaderFactory
{
public:
    TestReaderFactory(const std::string& extension):
        _extension(extension)
    {
    }

    std::string getName() const override
    {
        return "TEST " + _extension;
    }

    std::unique_ptr<BaseReader> makeReader(const std::string& file_name,
                                           std::shared_ptr<SampleFactory> sample_factory,
                                           std::shared_ptr<StreamTypeFactory> stream_type_factory,
                                           bool ignore_unsupported_streams) const override
    {
        if (file_name.find("." + _extension) == std::string::npos)
        {
            throw std::runtime_error("unsupported file type: " + file_name);
        }
        return std::make_unique<TestReader>(file_name);
    }
private:
    std::string _extension;
};

static Objects oObjects;
static PluginInitializer oInitializer([]
                                      {
                                          adtf_file::getObjects().push_back(std::make_shared<TestReaderFactory>("ext1"));
                                          adtf_file::getObjects().push_back(std::make_shared<TestReaderFactory>("ext2"));
                                          adtf_file::getObjects().push_back(std::make_shared<TestReaderFactory>("ext3"));
                                      });


GTEST_TEST(TestFactories, BaseReader)
{
    auto factories = getFactories<ReaderFactories, ReaderFactory>();
    ASSERT_NE(factories.makeReader("foo.ext1"), nullptr);
    ASSERT_NE(factories.makeReader("foo.ext2"), nullptr);
    ASSERT_NE(factories.makeReader("foo.ext3"), nullptr);

    try
    {
        factories.makeReader("foo.ext4");
        ASSERT_TRUE(false);
    }
    catch (const std::exception& error)
    {
        std::string helper(error.what());
        ASSERT_NE(helper.find("TEST ext1: unsupported file type: foo.ext4"), std::string::npos);
        ASSERT_NE(helper.find("TEST ext2: unsupported file type: foo.ext4"), std::string::npos);
        ASSERT_NE(helper.find("TEST ext1: unsupported file type: foo.ext4"), std::string::npos);
    }
}
