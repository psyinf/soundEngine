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


soundEngineX::Buffer::Buffer(const FormatDescriptor &format, const std::vector<char> &data)
  : Buffer()
{
    setData(format, data);
}

soundEngineX::Buffer::Buffer(Buffer &&other) noexcept
  : buffer(other.buffer)
  , format(other.format)
{
    other.buffer = 0;
}

soundEngineX::Buffer::Buffer() noexcept
{
    alCallImpl(alGenBuffers, 1, &buffer);
}

soundEngineX::Buffer::~Buffer()
{
    if (buffer != 0)
    {
        alCallImpl(alDeleteBuffers, 1, &buffer);
    }
}

void soundEngineX::Buffer::setData(const FormatDescriptor &format, const std::vector<char> &data)
{
    this->format = format;
    auto al_format = determineFormatType();
    alCallImpl(alBufferData, buffer, al_format, data.data(), data.size(), format.sampleRateHz);
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
