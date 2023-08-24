#pragma once
#include <AL/al.h>
#include <cstdint>
#include <vector>

namespace soundEngineX {


struct FormatDescriptor
{
    std::uint32_t sampleRateHz{};
    std::uint8_t channels{};
    std::uint8_t bitsPerSample{};
};
class Buffer
{
    using FormatType = int;
public:
    Buffer(const FormatDescriptor &format, const std::vector<char> &data);
    Buffer(Buffer &&other) noexcept;
    Buffer() noexcept;
    ~Buffer();
    Buffer& operator=(Buffer&& rhs) = delete;


    void setData(const FormatDescriptor &format, const std::vector<char> &data);

    const FormatDescriptor &getFormat() { return format; }
    operator ALuint() const { return buffer; }

protected:
    FormatType determineFormatType() const;

private:
    ALuint buffer{ 0 };
    FormatDescriptor format;
};

}// namespace soundEngineX
