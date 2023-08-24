#include "Buffer.h"
#include "helpers.h"

#include <sstream>

namespace internal {

ALenum getMonoFormat(uint8_t bitDepth)
{
    switch (bitDepth)
    {
    case 8:
        return AL_FORMAT_MONO8;
        break;
    case 16:
        return AL_FORMAT_MONO16;
        break;
    default:
        throw std::invalid_argument("Unsupported bit depth for mono: " + std::to_string(bitDepth));
        break;
    }
}

ALenum getStereoFormat(uint8_t bitDepth)
{
    switch (bitDepth)
    {
    case 8:
        return AL_FORMAT_STEREO8;
        break;
    case 16:
        return AL_FORMAT_STEREO16;
        break;
    default:
        throw std::invalid_argument("Unsupported bit depth for stereo: " + std::to_string(bitDepth));
        break;
    }
}
}// namespace internal


soundEngineX::Buffer::Buffer(const FormatDescriptor &format, const DataDescriptor &data)
  : Buffer()
{
    setData(format, data);
}

soundEngineX::Buffer::Buffer(Buffer &&other) noexcept
  : buffers(std::move(other.buffers))
  , format(other.format)
{
    other.buffers = {};
}

soundEngineX::Buffer::Buffer() noexcept
  : buffers(1)
{
    alCallImpl(alGenBuffers, buffers.size(), buffers.data());
}

soundEngineX::Buffer::~Buffer()
{
    alCallImpl(alDeleteBuffers, buffers.size(), buffers.data());
}

void soundEngineX::Buffer::setData(const FormatDescriptor &format, const DataDescriptor &data)
{
    this->format = format;
    auto al_format = determineFormatType();
    for (auto i = 0; i < data.chunks.size(); ++i)
    {
        //TODO: handle empty buffers
        alCallImpl(
          alBufferData, buffers[i], al_format, data.chunks[i].data(), data.chunks[i].size(), format.sampleRateHz);
    }
}


const std::vector<ALuint> &soundEngineX::Buffer::getHandles() const
{
    return buffers;
}

soundEngineX::Buffer::FormatType soundEngineX::Buffer::determineFormatType() const
{
    switch (format.channels)
    {
    case 1:
        return internal::getMonoFormat(format.bitsPerSample);
        break;
    case 2:
        return internal::getStereoFormat(format.bitsPerSample);
        break;
    default:
        throw std::invalid_argument("Unsupported number of channels: " + std::to_string(format.channels));
        break;
    }
}
