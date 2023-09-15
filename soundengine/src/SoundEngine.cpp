#include "SoundEngine.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>

#include "ALHelpers.h"
#include "LoadWave.h"
#include <Buffer.h>
#include <Loader.h>
using namespace soundEngineX;

SoundEngine::SoundEngine()
{
    init();
}

SoundEngine::~SoundEngine()
{
    if (ALCboolean closed; !alcCallImpl(alcCloseDevice, closed, device, device))
    { /* do we care? */
    }
    if (ALCboolean contextMadeCurrent; !alcCallImpl(alcMakeContextCurrent, contextMadeCurrent, device, nullptr))
    { /* what can you do? */
    }

    if (!alcCallImpl(alcDestroyContext, device, context))
    { /* not much you can do */
    }
}

void SoundEngine::init()
{
    device = {alcOpenDevice(nullptr)};

    if (!alcCallImpl(alcCreateContext, context, device, device, nullptr) || !context)
    {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
        /* probably exit program */
        throw std::runtime_error("Could not create audio context");
    }

    ALCboolean contextMadeCurrent = false;
    if (!alcCallImpl(alcMakeContextCurrent, contextMadeCurrent, device, context) || contextMadeCurrent != ALC_TRUE)
    {
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
        /* probably exit or give up on having sound */
        throw std::runtime_error("Could not make audio context current");
    }

    // alcCallImpl(alcIsExtensionPresent(ALCdevice * device, const ALCchar *extName);
}

void SoundEngine::iterateDevices(Device& device)
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
