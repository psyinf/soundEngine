#pragma once
#include <bit>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>

#include <Buffer.h>
#include <cstring>
using namespace soundEngineX;

static void load_wav_file_header(std::istream& file, FormatDescriptor& format, size_t& size)
{
    struct header
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

    file.read(reinterpret_cast<char*>(&header), 44); // TODO: replace 44 with size of struct?
    // TODO: check header values

    format.channels = header.numChannels;
    format.bitsPerSample = header.bitsPerSample;
    format.sampleRateHz = header.samplesPerSec;
    size = header.dataSize;
}

static std::tuple<std::vector<char>, FormatDescriptor> load_wav(std::istream& in)
{
    size_t           size = 0;
    FormatDescriptor format;
    load_wav_file_header(in, format, size);
    std::vector<char> data(size);
    in.read(data.data(), size);

    return {data, format};
}

static std::tuple<std::vector<char>, FormatDescriptor> load_wav(const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        throw std::runtime_error("Could not open \"" + filename + "\"");
    }

    return load_wav(in);
}