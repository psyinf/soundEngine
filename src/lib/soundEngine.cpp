#include "soundEngine.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>

#include "helpers.h"
#include "loadWave.h"
using namespace soundEngineX;


SoundEngine::SoundEngine() { init(); }

SoundEngine::~SoundEngine()
{
    if (ALCboolean closed; !alcCallImpl(alcCloseDevice, closed, device, device)) { /* do we care? */
    }
    if (ALCboolean contextMadeCurrent; !alcCallImpl(
          alcMakeContextCurrent, contextMadeCurrent, device, nullptr)) { /* what can you do? */
    }

    if (!alcCallImpl(alcDestroyContext, device, context)) { /* not much you can do */
    }
}

void SoundEngine::init()
{
    device = { alcOpenDevice(nullptr) };

    if (!alcCallImpl(alcCreateContext, context, device, device, nullptr) || !context) {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
        /* probably exit program */
        throw std::runtime_error("Could not create audio context");
    }

    ALCboolean contextMadeCurrent = false;
    if (!alcCallImpl(alcMakeContextCurrent, contextMadeCurrent, device, context) || contextMadeCurrent != ALC_TRUE) {
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
        /* probably exit or give up on having sound */
        throw std::runtime_error("Could not make audio context current");
    }

    //////////////////////////////////////////////////////////////////////////
    ALuint buffer;
    alCallImpl(alGenBuffers, 1, &buffer);
    std::uint8_t channels;
    std::int32_t sampleRate;
    std::uint8_t bitsPerSample;
    std::vector<char> soundData;
    if (!load_wav("data/test.wav", channels, sampleRate, bitsPerSample, soundData)) {
        std::cerr << "ERROR: Could not load wave file" << std::endl;
        throw std::runtime_error("Could not load wave file");
    }

    ALenum format;
    if (channels == 1 && bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if (channels == 1 && bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if (channels == 2 && bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if (channels == 2 && bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else {
        std::cerr << "ERROR: unrecognized wave format: " << channels << " channels, " << bitsPerSample << " bps"
                  << std::endl;
        throw std::runtime_error("Could not load wave file");
    }

    alCallImpl(alBufferData, buffer, format, soundData.data(), soundData.size(), sampleRate);
    soundData.clear();// erase the sound in RAM

    ALuint source;
    alCallImpl(alGenSources, 1, &source);
    alCallImpl(alSourcef, source, AL_PITCH, 1);
    alCallImpl(alSourcef, source, AL_GAIN, 1.0f);
    alCallImpl(alSource3f, source, AL_POSITION, 0, 0, 0);
    alCallImpl(alSource3f, source, AL_VELOCITY, 0, 0, 0);
    alCallImpl(alSourcei, source, AL_LOOPING, AL_FALSE);
    alCallImpl(alSourcei, source, AL_BUFFER, buffer);

    alCallImpl(alSourcePlay, source);

    ALint state = AL_PLAYING;

    while (state == AL_PLAYING) { alCallImpl(alGetSourcei, source, AL_SOURCE_STATE, &state); }
    /*
    alCallImpl(alDeleteSources, 1, &source);
    alCallImpl(alDeleteBuffers, 1, &buffer);

    alcCallImpl(alcMakeContextCurrent, contextMadeCurrent, device, nullptr);
    alcCallImpl(alcDestroyContext, device, context);

    ALCboolean closed;
    alcCallImpl(alcCloseDevice, closed, device, device);
    */

}


void SoundEngine::iterateDevices(Device &device)
{
    // LATER:
    /*
    const ALCchar *devices;

    if (alcCallImpl(alcGetString, devices, device, nullptr, ALC_ALL_DEVICES_SPECIFIER)) {

         auto split = std | std::ranges::views::split(' ') | std::ranges::views::transform([](auto &&str) {
            return std::string_view(&*str.begin(), std::ranges::distance(str));
        });

        for (auto &&word : split) { std::cout << word << std::endl; }
    }
    */
}
