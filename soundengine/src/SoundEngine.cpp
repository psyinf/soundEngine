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
    std::vector<std::string> devices;
    iterateDevices(devices);
    spdlog::debug("Available devices:");
    for (const auto& device : devices)
    {
        spdlog::debug("Device: {}", device);
    }
    init();
}

SoundEngine::~SoundEngine()
{
    spdlog::info("Destroying sound engine");
    if (ALCboolean contextMadeCurrent; !alcCallImpl(alcMakeContextCurrent, contextMadeCurrent, device, nullptr))
    { /* what can you do? */
    }

    if (!alcCallImpl(alcDestroyContext, device, context.context))
    { /* not much you can do */
    }
    if (ALCboolean closed; !alcCallImpl(alcCloseDevice, closed, device, device))
    { // do we care?
    }
    spdlog::info("Sound engine destroyed");
}

void SoundEngine::init()
{
    device = {alcOpenDevice(nullptr)};

    if (!alcCallImpl(alcCreateContext, context, device, device, nullptr) || !context)
    {
        /* probably exit program */
        throw std::runtime_error("Could not create audio context");
    }

    ALCboolean contextMadeCurrent = false;
    if (!alcCallImpl(alcMakeContextCurrent, contextMadeCurrent, device, context) || contextMadeCurrent != ALC_TRUE)
    {
        /* probably exit or give up on having sound */
        throw std::runtime_error("Could not make audio context current");
    }

    // alcCallImpl(alcIsExtensionPresent(ALCdevice * device, const ALCchar *extName);
}

void SoundEngine::iterateDevices(std::vector<std::string>& devicesVec)
{
    const ALCchar* devices;
    if (!alcCallImpl(alcGetString, devices, device, nullptr, ALC_DEVICE_SPECIFIER)) return;

    const char* ptr = devices;

    do
    {
        devicesVec.push_back(std::string(ptr));
        ptr += devicesVec.back().size() + 1;
    } while (*(ptr + 1) != '\0');
}
