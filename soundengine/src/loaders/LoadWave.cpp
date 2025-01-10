#include "LoadWave.hpp"
#include <fmt/format.h>

soundEngineX::FormatDescriptor load_wav_file_header(const std::vector<char>& buffer, size_t& size)
{
    soundEngineX::FormatDescriptor formatDesc;

    struct WAVHeader
    {
        char          chunkID[4];
        std::uint32_t fileSize;
        char          waveHeader[4];
        char          fmt[4];
        std::uint32_t lenFormatData;
        std::uint16_t fmtType;
        std::uint16_t numChannels;
        std::uint32_t samplesPerSec;
        std::uint32_t avgBytesPerSec;
        std::uint16_t blockAlign;
        std::uint16_t bitsPerSample;
        char          data[4];
        std::uint32_t dataSize;
    } header;

    if (buffer.size() < 44) { throw std::runtime_error("Invalid WAV file header"); }
    std::memcpy(&header, buffer.data(), sizeof(WAVHeader));

    // check
    if (std::memcmp(header.chunkID, "RIFF", 4) != 0) { throw std::runtime_error("Invalid WAV file header"); }

    formatDesc.channels = static_cast<std::uint8_t>(header.numChannels);
    formatDesc.bitsPerSample = static_cast<std::uint8_t>(header.bitsPerSample);
    formatDesc.sampleRateHz = header.samplesPerSec;
    size = header.dataSize;

    return formatDesc;
}

soundEngineX::DataChunk soundEngineX::format::load_wav(std::istream&                         in,
                                                       soundEngineX::loader::LoadingCallback progress_cb)
{
    size_t size = 0;
    // header into vector
    std::vector<char> header(44);
    in.read(header.data(), 44);

    FormatDescriptor  format = load_wav_file_header(header, size);
    std::vector<char> data(size);
    // read chunked if size is greater than threshold (e.g. 1MB)
    // every 1 MB read, call progress_cb
    size_t chunkSize = 1024 * 1024;
    if (size > chunkSize && progress_cb.cb)
    {
        size_t read = 0;
        while (read < size)
        {
            auto toRead = std::min(chunkSize, size - read);
            in.read(data.data() + read, toRead);
            read += toRead;
            progress_cb.cb({read, size});
        }
    }
    else
    {
        in.read(data.data(), size);
        if (progress_cb.cb) { progress_cb.cb({size, size}); }
    }

    return {format, data};
}

soundEngineX::DataChunk soundEngineX::format::load_wav(std::string_view                      filename,
                                                       soundEngineX::loader::LoadingCallback progress_cb)
{
    std::ifstream in(filename.data(), std::ios::binary);
    if (!in.is_open()) { throw std::runtime_error(fmt::format("Could not open {} for reading", filename)); }

    return load_wav(in, progress_cb);
}

soundEngineX::DataChunk soundEngineX::format::load_wav(const std::vector<char>&              buffer,
                                                       soundEngineX::loader::LoadingCallback progress_cb)
{
    size_t            size = 0;
    FormatDescriptor  format = load_wav_file_header(buffer, size);
    std::vector<char> data(size);
    std::memcpy(data.data(), buffer.data() + 44, size);
    if (progress_cb.cb) { progress_cb.cb({size, size}); }
    return {format, data};
}
