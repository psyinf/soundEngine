#pragma once
#include <AL/al.h>
#include <Buffer.h>
#include <cstdint>
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
    using DataChunk = std::vector<char>;

    DataDescriptor() = default;
    explicit DataDescriptor(size_t size)
      : chunks(size)
    {}

    std::vector<DataChunk> chunks;
};

class Buffer
{
    using FormatType = int;

public:
    Buffer(const FormatDescriptor &format, const DataDescriptor &data);

    Buffer(Buffer &&other) noexcept;
    explicit Buffer(size_t num_chunks = 1) noexcept;
    ~Buffer();

    Buffer(Buffer &rhs) = delete;
    Buffer &operator=(Buffer &&rhs) = delete;
    Buffer &operator=(Buffer &rhs) = delete;

    void setData(const FormatDescriptor &format, const DataDescriptor &data);
    std::vector<ALuint> bufferUnqueued(ALuint buffer);
    const FormatDescriptor &getFormat() { return format; }
    const std::vector<ALuint> &getHandles() const;


protected:
    FormatType determineFormatType() const;

private:
    std::vector<ALuint> buffers;
    FormatDescriptor format;
};

}// namespace soundEngineX
