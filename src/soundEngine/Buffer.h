#pragma once
#include <AL/al.h>
#include <Buffer.h>
#include <cstdint>
#include <functional>
#include <vector>


namespace soundEngineX {


struct FormatDescriptor
{
    std::uint32_t sampleRateHz{};
    std::uint8_t channels{};
    std::uint8_t bitsPerSample{};
};

struct DataDescriptor
{
    using RequestDataFunction = std::function<DataDescriptor>(size_t);
    using DataChunk = std::vector<char>;
    using Chunks = std::vector<DataChunk>;
    DataDescriptor() = default;
    explicit DataDescriptor(size_t size)
      : chunks(size)
    {}

    FormatDescriptor format;
    Chunks chunks;
    
    RequestDataFunction requestMoreDataCallback;
};

class Buffer
{
    using FormatType = int;

public:
    Buffer(const DataDescriptor &data);

    Buffer(Buffer &&other) noexcept;
    explicit Buffer(size_t num_chunks = 1);
    ~Buffer();

    Buffer(Buffer &rhs) = delete;
    Buffer &operator=(Buffer &&rhs) = delete;
    Buffer &operator=(Buffer &rhs) = delete;

   
    std::vector<ALuint> bufferUnqueued(ALuint buffer);
    const FormatDescriptor &getFormat() { return format; }
    const std::vector<ALuint> &getHandles() const;


protected:
    FormatType determineFormatType() const;
    void setData(const DataDescriptor &data);

private:
    std::vector<ALuint> buffers;

    FormatDescriptor format;
    DataDescriptor::RequestDataFunction requestMoreDataCallback;
};

}// namespace soundEngineX
