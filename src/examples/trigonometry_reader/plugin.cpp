#include "trigonomentry_reader.h"

// this is a factory that creates instances of our reader implementation
class Factory: public adtf_file::ReaderFactory
{
    std::string getName() const
    {
        return "trigonometry";
    }

    std::unique_ptr<adtf_file::BaseReader> makeReader(const std::string& file_name,
                                                      std::shared_ptr<adtf_file::SampleFactory> sample_factory,
                                                      std::shared_ptr<adtf_file::StreamTypeFactory> stream_type_factory,
                                                      bool /*ignore_unsupported_streams*/) const
    {
        return std::make_unique<TrigonometryReader>(file_name, sample_factory, stream_type_factory);
    }
};

// make the factory available via the global object list during plugin initialiization.
static adtf_file::PluginInitializer initializer([]
{
    adtf_file::getObjects().push_back(std::make_shared<Factory>());
});
