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


soundEngineX::Buffer::Buffer(const DataDescriptor &data)
  : Buffer(data.chunks.size())
{
    setData(data);
}

soundEngineX::Buffer::Buffer(Buffer &&other) noexcept
  : buffers(std::move(other.buffers))
  , format(other.format)
{
    other.buffers = {};
}

soundEngineX::Buffer::Buffer(size_t num_chunks)
  : buffers(num_chunks)
{
    if (num_chunks == 0)
    {
        throw std::invalid_argument("Number of chunks must be greater than 0");
    }
    alCallImpl(alGenBuffers, buffers.size(), buffers.data());
}

soundEngineX::Buffer::~Buffer()
{
    alCallImpl(alDeleteBuffers, buffers.size(), buffers.data());
}

void soundEngineX::Buffer::setData(const DataDescriptor &data)
{
    this->format = data.format;
    auto al_format = determineFormatType();
    for (auto i = 0; i < data.chunks.size(); ++i)
    {
        //TODO: handle empty buffers
        alCallImpl(
          alBufferData, buffers[i], al_format, data.chunks[i].data(), data.chunks[i].size(), format.sampleRateHz);
    }
}


std::vector<ALuint> soundEngineX::Buffer::bufferUnqueued(ALuint buffer)
{
    //if stream data source
    //request next buffer chunk(s) and report them
    return {0};
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
