#include "Loader.h"
#include "loadWave.h"

namespace soundEngineX::loader {

std::unique_ptr<Buffer> Loader::load(std::istream &&stream, Type type)
{
    FormatDescriptor format;
    switch (type)
    {
    case soundEngineX::loader::Type::WAV: {
        return std::make_unique<Buffer>(load_wav(stream) );
    }
    break;
    default:
        throw std::invalid_argument("Unknown format");
        break;
    }
    
}
std::unique_ptr<soundEngineX::Buffer> Loader::loadStream(std::istream &stream, Type type)
{
    FormatDescriptor format;
    switch (type)
    {
    case soundEngineX::loader::Type::WAV: {
        return std::make_unique<Buffer>(load_wav(stream));
    }
    break;
    default:
        throw std::invalid_argument("Unknown format");
        break;
    }
}
}// namespace soundEngineX::loader