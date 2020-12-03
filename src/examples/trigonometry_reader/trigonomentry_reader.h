#pragma once

#include <adtf_file/base_reader.h>
#include <memory>
#include <chrono>

class TrigonometryReader : public adtf_file::BaseReader
{
public:
    TrigonometryReader(const std::string& url,
              std::shared_ptr<adtf_file::SampleFactory> sample_factory,
              std::shared_ptr<adtf_file::StreamTypeFactory> stream_type_factory);

    uint32_t getFileVersion() const override;
    a_util::datetime::DateTime getDateTime() const override;
    std::chrono::nanoseconds getFirstTime() const override;
    std::chrono::nanoseconds getLastTime() const override;
    std::chrono::nanoseconds getDuration() const override;
    std::string getDescription() const override;

    const std::vector<adtf_file::Extension>& getExtensions() const override;
    const std::vector<adtf_file::Stream>& getStreams() const override;
    uint64_t getItemCount() const override;

    uint64_t getItemIndexForTimeStamp(std::chrono::nanoseconds time_stamp) override;
    uint64_t getItemIndexForStreamItemIndex(uint16_t stream_id, uint64_t stream_item_index) override;
    std::shared_ptr<const adtf_file::StreamType> getStreamTypeBefore(uint64_t item_index, uint16_t stream_id, bool update_sample_deserializer) override;

    void seekTo(uint64_t item_index) override;
    adtf_file::FileItem getNextItem() override;
    int64_t getNextItemIndex() override;

private:
    std::shared_ptr<adtf_file::SampleFactory> _sample_factory;
    std::vector<adtf_file::Stream> _streams;

    a_util::datetime::DateTime _date_time;
    using double_seconds = std::chrono::duration<double>;
    std::chrono::nanoseconds _last_timestamp;
    double_seconds _period_length;
    double_seconds _interval;
    uint64_t _item_count;

    uint64_t _current_item_index = 0;
};
