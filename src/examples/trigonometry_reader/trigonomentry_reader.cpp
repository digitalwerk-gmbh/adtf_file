#include "trigonomentry_reader.h"
#include <sstream>
#include <string>
#include <cmath>
#include <regex>

std::string build_ddl(const std::string& name)
{
    std::ostringstream struct_definition;
    struct_definition << R"(<struct name=")" << name << R"(" alignment="1" version="1">)" <<
R"(
  <element name="timestamp" type="tFloat64" arraysize="1">
    <deserialized alignment="1"/>
    <serialized bytepos="0" byteorder="LE"/>
  </element>
</struct>
)";

    return struct_definition.str();
}

std::shared_ptr<adtf_file::StreamType> build_type(adtf_file::StreamTypeFactory& stream_type_factory, const std::string& name)
{
    auto type = stream_type_factory.build();
    auto property_type = std::dynamic_pointer_cast<adtf_file::PropertyStreamType>(type);
    property_type->setMetaType("adtf/default");
    property_type->setProperty("md_definitions", "cString", build_ddl(name));
    property_type->setProperty("md_struct", "cString", name);
    property_type->setProperty("md_data_serialized", "tBool", "false");
    return type;
}

TrigonometryReader::TrigonometryReader(const std::string& url,
                     std::shared_ptr<adtf_file::SampleFactory> sample_factory,
                     std::shared_ptr<adtf_file::StreamTypeFactory> stream_type_factory):
    _sample_factory(sample_factory),
    _date_time(a_util::datetime::getCurrentSystemDateTime())
{
    // we get our parameters from the url in the form of trigonometry://<duration>/<period_length>/<interval>
    std::regex url_regex("trigonometry://([\\d\\.,]+)/([\\d\\.,]+)/([\\d\\.,]+)");
    std::smatch matches;
    if (!std::regex_search(url, matches, url_regex))
    {
        throw std::runtime_error("unable to parse url: " + url);
    }

    _last_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(double_seconds(std::stod(matches[1])));
    _period_length = double_seconds(std::stod(matches[2]));
    _interval = double_seconds(std::stod(matches[3]));

    auto interval_count = static_cast<uint64_t>(_last_timestamp / _interval);

    // stream ids have to start at 1, 0 is reserved as "all streams" id
    _streams.push_back({1,
                        "sine",
                        interval_count,
                        std::chrono::seconds(0),
                        _last_timestamp,
                        build_type(*stream_type_factory, "sine")});

    _streams.push_back({2,
                        "cosine",
                        interval_count,
                        std::chrono::seconds(0),
                        _last_timestamp,
                        build_type(*stream_type_factory, "cosine")});

    _streams.push_back({3,
                        "tangent",
                        interval_count,
                        std::chrono::seconds(0),
                        _last_timestamp,
                        build_type(*stream_type_factory, "tangent")});

    _item_count = interval_count * _streams.size();
}

uint32_t TrigonometryReader::getFileVersion() const
{
    return ifhd::v201_v301::version_id; // We identify as an ADTF 2 Dat File. This way we do not have to emit trigger items as well.
}

a_util::datetime::DateTime TrigonometryReader::getDateTime() const
{
    return _date_time;
}

std::chrono::nanoseconds TrigonometryReader::getFirstTime() const
{
    return std::chrono::nanoseconds{0};
}

std::chrono::nanoseconds TrigonometryReader::getLastTime() const
{
    return _last_timestamp;
}

std::chrono::nanoseconds TrigonometryReader::getDuration() const
{
    return _last_timestamp;
}

std::string TrigonometryReader::getDescription() const
{
    return "";
}

const std::vector<adtf_file::Extension>& TrigonometryReader::getExtensions() const
{
    static std::vector<adtf_file::Extension> no_extentions;
    return no_extentions;
}

const std::vector<adtf_file::Stream>& TrigonometryReader::getStreams() const
{
    return _streams;
}

uint64_t TrigonometryReader::getItemCount() const
{
    return _item_count;
}

uint64_t TrigonometryReader::getItemIndexForTimeStamp(std::chrono::nanoseconds time_stamp)
{
    return time_stamp / _interval * _streams.size();
}

uint64_t TrigonometryReader::getItemIndexForStreamItemIndex(uint16_t stream_id, uint64_t stream_item_index)
{
    return stream_item_index * 3 + (stream_id - 1);
}

std::shared_ptr<const adtf_file::StreamType> TrigonometryReader::getStreamTypeBefore(uint64_t /*item_index*/,
                                                                            uint16_t stream_id,
                                                                            bool /*update_sample_deserializer*/)
{
    return _streams.at(stream_id - 1).initial_type;
}

void TrigonometryReader::seekTo(uint64_t item_index)
{
    if (item_index >= _item_count)
    {
        throw std::runtime_error("invalid item index: " + std::to_string(item_index));
    }

    _current_item_index = item_index;
}

adtf_file::FileItem TrigonometryReader::getNextItem()
{
    if (_current_item_index == _item_count)
    {
        throw adtf_file::exceptions::EndOfFile();
    }

    // calculate the value
    auto timestamp = _interval * (_current_item_index / 3);
    auto stream_id = (_current_item_index % _streams.size()) + 1;

    double angle = timestamp / _period_length * M_PI * 2;
    double value = 0.0;
    switch (stream_id)
    {
       case 1: value = sin(angle); break;
       case 2: value = cos(angle); break;
       case 3: value = tan(angle); break;
       default: throw std::runtime_error("unexpected stream id");
    }

    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(timestamp);

    // create a new sample and copy the value to it.
    auto sample = _sample_factory->build();
    auto read_sample = std::dynamic_pointer_cast<adtf_file::ReadSample>(sample);
    read_sample->setTimeStamp(nanoseconds);
    auto buffer = read_sample->beginBufferWrite(sizeof(double));
    *static_cast<double*>(buffer) = value;
    read_sample->endBufferWrite();

    ++_current_item_index;

    return {static_cast<uint16_t>(stream_id), nanoseconds, sample};
}

int64_t TrigonometryReader::getNextItemIndex()
{
    return _current_item_index;
}
