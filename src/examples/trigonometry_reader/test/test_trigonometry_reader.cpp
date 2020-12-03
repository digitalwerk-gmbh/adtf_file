#include <gtest/gtest.h>
#include <adtf_file/base_reader.h>

using namespace adtf_file;

static Objects objects;

GTEST_TEST(TrigonometryReader, sample_generation)
{
    loadPlugin(TRIGONOMETRY_READER_PLUGIN);

    auto reader_factories = getFactories<ReaderFactories, ReaderFactory>();
    auto trigonometry_reader = reader_factories.makeReader("trigonometry://10/1/0.1");

    ASSERT_EQ(trigonometry_reader->getItemCount(), 300);
    ASSERT_EQ(trigonometry_reader->getFirstTime(), std::chrono::seconds{0});
    ASSERT_EQ(trigonometry_reader->getLastTime(), std::chrono::seconds{10});

    const auto& streams = trigonometry_reader->getStreams();
    ASSERT_EQ(streams.size(), 3);
    ASSERT_EQ(streams[0].name, "sine");
    ASSERT_EQ(streams[0].item_count, 100);
    ASSERT_EQ(streams[1].name, "cosine");
    ASSERT_EQ(streams[1].item_count, 100);
    ASSERT_EQ(streams[2].name, "tangent");
    ASSERT_EQ(streams[2].item_count, 100);

    std::chrono::nanoseconds expected_timestamp{0};
    for (size_t item_index = 0; item_index < trigonometry_reader->getItemCount(); ++item_index)
    {
        auto item = trigonometry_reader->getNextItem();
        ASSERT_EQ(item.stream_id, (item_index % 3) + 1);
        ASSERT_EQ(item.time_stamp, expected_timestamp);

        if (item_index % 3 == 2)
        {
            expected_timestamp += std::chrono::milliseconds(100);
        }
    }
}
