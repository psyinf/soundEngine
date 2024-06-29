#pragma once
#include <AL/al.h>

#include <cstdint>
#include <deque>
#include <functional>
#include <vector>
#include <mutex>

namespace soundEngineX {

struct FormatDescriptor
{
    std::uint32_t sampleRateHz{};
    std::uint8_t  channels{};
    std::uint8_t  bitsPerSample{};
};

struct DataChunk
{
    FormatDescriptor  format;
    std::vector<char> data;
};

struct DataDescriptor
{
    using RequestDataFunction = std::function<DataDescriptor(size_t)>;
    using Chunks = std::vector<DataChunk>;

    Chunks chunks;

    RequestDataFunction requestMoreDataCallback;
};

class Buffer
{
    using FormatType = int;

public:
    Buffer(const DataDescriptor& data);
    Buffer(DataDescriptor&& data);
    Buffer(Buffer&& other) noexcept;
    explicit Buffer(size_t num_chunks = 1);
    ~Buffer();

    Buffer(Buffer& rhs) = delete;
    Buffer& operator=(Buffer&& rhs) = delete;
    Buffer& operator=(Buffer& rhs) = delete;

    std::vector<ALuint>        buffersUnqueued(const std::vector<ALuint>& unqueuedBuffers);
    const std::vector<ALuint>& getHandles() const;

    void setRequestNewDataCallback(DataDescriptor::RequestDataFunction requestDataFunction)
    {
        requestMoreDataCallback = requestDataFunction;
    }

    std::chrono::high_resolution_clock::duration getDurationEstimation() const;

private:
    void setBufferData(const auto& chunk, ALuint targetBuffer);

protected:
    FormatType determineFormatType(const FormatDescriptor& format) const;

private:
    std::vector<ALuint> buffers;     //> AL buffer handles allocated by this buffer
    std::deque<ALuint>  freeBuffers; //> AL buffer handles that are free to be used

    DataDescriptor::RequestDataFunction requestMoreDataCallback;
    std::mutex                          dataCallbackMutex;
};

} // namespace soundEngineX
