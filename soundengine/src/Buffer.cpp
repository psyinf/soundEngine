#include <sndX/Buffer.hpp>
#include <sndX/ALHelpers.hpp>

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
} // namespace internal

soundEngineX::Buffer::Buffer(const DataDescriptor& data)
  : Buffer(data.chunks.size())
{
    requestMoreDataCallback = (data.requestMoreDataCallback);
    for (auto i = 0; i < data.chunks.size(); ++i)
    {
        alCallImpl(alBufferData,
                   buffers[i],
                   determineFormatType(data.chunks[i].format),
                   data.chunks[i].data.data(),
                   static_cast<ALsizei>(data.chunks[i].data.size()),
                   data.chunks[i].format.sampleRateHz);
    }
}

soundEngineX::Buffer::Buffer(DataDescriptor&& data)
  : Buffer(data.chunks.size())
{
    requestMoreDataCallback = (data.requestMoreDataCallback);
    for (auto i = 0; i < data.chunks.size(); ++i)
    {
        setBufferData(data.chunks[i], buffers[i]);
    }
}

soundEngineX::Buffer::Buffer(Buffer&& other) noexcept
  : buffers(std::move(other.buffers))
{
    other.buffers = {};
}

soundEngineX::Buffer::Buffer(size_t num_chunks)
  : buffers(num_chunks)
{
    if (num_chunks == 0) { throw std::invalid_argument("Number of chunks must be greater than 0"); }
    alCallImpl(alGenBuffers, static_cast<ALsizei>(buffers.size()), buffers.data());
}

soundEngineX::Buffer::~Buffer()
{
    // filter out already deleted buffers
    std::vector<ALuint> validBuffers;
    for (const auto& buffer : buffers)
    {
        if (alIsBuffer(buffer)) { validBuffers.push_back(buffer); }
    }
    if (validBuffers.empty()) { return; }
    alCallImpl(alDeleteBuffers, static_cast<ALsizei>(validBuffers.size()), validBuffers.data());
}

std::vector<ALuint> soundEngineX::Buffer::buffersUnqueued(const std::vector<ALuint>& unqueuedBuffers)
{
    // lock mutex
    std::lock_guard<std::mutex> lock(dataCallbackMutex);
    freeBuffers.insert(freeBuffers.end(), unqueuedBuffers.begin(), unqueuedBuffers.end());

    size_t newBuffers = 0;

    if (requestMoreDataCallback)
    {
        auto data = requestMoreDataCallback(unqueuedBuffers.size());
        newBuffers = data.chunks.size();
        for (const auto& chunk : data.chunks)
        {
            setBufferData(chunk, freeBuffers.front());
            freeBuffers.pop_front();
        }
    }
    return {unqueuedBuffers.begin(), unqueuedBuffers.begin() + newBuffers};
}

void soundEngineX::Buffer::setBufferData(const auto& chunk, ALuint targetBuffer)
{
    alCallImpl(alBufferData,
               targetBuffer,
               determineFormatType(chunk.format),
               chunk.data.data(),
               static_cast<ALsizei>(chunk.data.size()),
               chunk.format.sampleRateHz);
}

const std::vector<ALuint>& soundEngineX::Buffer::getHandles() const
{
    return buffers;
}

std::chrono::high_resolution_clock::duration soundEngineX::Buffer::getDurationEstimation() const
{
    // for all buffers, get the duration
    std::chrono::high_resolution_clock::duration duration = std::chrono::high_resolution_clock::duration::zero();
    for (const auto& buffer : buffers)
    {
        ALint size, bitsPerSample, channels, frequency;
        alCallImpl(alGetBufferi, buffer, AL_SIZE, &size);
        alCallImpl(alGetBufferi, buffer, AL_BITS, &bitsPerSample);
        alCallImpl(alGetBufferi, buffer, AL_CHANNELS, &channels);
        alCallImpl(alGetBufferi, buffer, AL_FREQUENCY, &frequency);

        duration += std::chrono::milliseconds(
            static_cast<int>(1000 * static_cast<float>(size) / (channels * (bitsPerSample / 8) * frequency)));
    }
    return duration;
}

soundEngineX::Buffer::FormatType soundEngineX::Buffer::determineFormatType(const FormatDescriptor& format) const
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
