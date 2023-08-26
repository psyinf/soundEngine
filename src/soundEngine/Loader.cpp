#include "Loader.h"
#include "loadWave.h"

namespace soundEngineX::loader {

std::unique_ptr<Buffer> Loader::load(std::istream &&stream, Type type)
{
   switch (type)
    {
    case soundEngineX::loader::Type::WAV: {
        auto &&[data, format] = load_wav(stream); 
        return std::make_unique<Buffer>(DataDescriptor{ .format=format, .chunks={ data } });
    }
    break;
    default:
        throw std::invalid_argument("Unknown format");
        break;
    }
    
}

std::unique_ptr<Buffer> Loader::fakeStream(const std::vector<std::string> &names)
{
    DataDescriptor ds{};

    for (auto file : names)
    {
        auto &&[data, format] = load_wav(file);
        ds.format = format;
        ds.chunks.push_back(data);
    }
  
    return std::make_unique<Buffer>(ds);
}
}// namespace soundEngineX::loader