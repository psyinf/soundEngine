#include "Buffer.h"
#include "helpers.h"

#include <sstream>
soundEngineX::Buffer::Buffer() { alCallImpl(alGenBuffers, 1, &buffer); }

soundEngineX::Buffer::~Buffer()
{
    alCallImpl(alDeleteBuffers, 1, &buffer);
}

void soundEngineX::Buffer::setData(FormatDescriptor &format, const std::vector<char> &data)
{
    ALenum al_format;

    if (format.channels == 1 && format.bitsPerSample == 8)
        al_format = AL_FORMAT_MONO8;
    else if (format.channels == 1 && format.bitsPerSample == 16)
        al_format = AL_FORMAT_MONO16;
    else if (format.channels == 2 && format.bitsPerSample == 8)
        al_format = AL_FORMAT_STEREO8;
    else if (format.channels == 2 && format.bitsPerSample == 16)
        al_format = AL_FORMAT_STEREO16;
    else {
        std::stringstream ss{};
        ss << "unrecognized wave format: " << format.channels << " channels, " << format.bitsPerSample << " bps"
           << "\n";
        throw std::runtime_error(ss.str());
    }

    alCallImpl(alBufferData, buffer, al_format, data.data(), data.size(), format.sampleRateHz);
}