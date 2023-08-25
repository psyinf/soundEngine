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
    // TODO: check header values

    format.channels = h.numChannels;
    format.bitsPerSample = h.bitsPerSample;
    format.sampleRateHz = h.samplesPerSec;
    size = h.dataSize;
}


static soundEngineX::DataDescriptor load_wav(std::istream &in)
{
    DataDescriptor data_desc = DataDescriptor(1);
   
    size_t size = 0;
    load_wav_file_header(in, data_desc.format, size);
    data_desc.chunks[0].resize(size);
       
    in.read(data_desc.chunks[0].data(), data_desc.chunks[0].size());
    
    return data_desc;
}

static soundEngineX::DataDescriptor load_wav(const std::string &filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        throw std::runtime_error("Could not open \"" + filename + "\"");
    }

    return load_wav(in);
}