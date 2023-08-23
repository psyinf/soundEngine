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

static void load_wav_file_header(std::istream &file, FormatDescriptor &format, size_t &size)
{
    struct header
    {
        char chunkID[4];
        std::uint32_t fileSize;
        char waveHeader[4];
        char fmt[4];
        std::uint32_t lenFormatData;
        std::uint16_t fmtType;
        std::uint16_t numChannels;
        std::uint32_t samplesPerSec;
        std::uint32_t avgBytesPerSec;
        std::uint16_t blockAlign;
        std::uint16_t bitsPerSample;
        char data[4];
        std::uint32_t dataSize;
    } h;
  
    file.read(reinterpret_cast<char *>(&h), 44);
    //TODO: check header values

    format.channels = h.numChannels;
    format.bitsPerSample = h.bitsPerSample;
    format.sampleRateHz = h.samplesPerSec;
    size = h.dataSize;
}

static std::vector<char> load_wav(std::istream &in, FormatDescriptor &format)
{
    size_t size = 0;
    load_wav_file_header(in, format, size);
    
    auto data = std::vector<char>(size);
    in.read(data.data(), size);
    return data;
}

static std::vector<char> load_wav(const std::string &filename, FormatDescriptor &format)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        throw std::runtime_error("Could not open \"" + filename + "\"");
    }

    return load_wav(in, format);
}