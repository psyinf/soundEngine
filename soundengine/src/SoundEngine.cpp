#include <sndX/SoundEngine.hpp>
#include <sndX/ALHelpers.hpp>
#include <sndX/SourceManager.hpp>

#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>

using namespace soundEngineX;

std::vector<std::string> alStringToVector(const ALCchar* list, const char separator)
{
    if (!list) return {};
    std::vector<std::string> resultVec;
    std::string_view         sv(list);
    auto                     splits = std::ranges::split_view(sv, {separator});
    // copy each split into the resultVec
    for (auto split : splits)
    {
        resultVec.push_back(std::string(std::begin(split), std::end(split)));
    }

    return resultVec;
}

SoundEngine::SoundEngine()
{
    spdlog::debug("Available devices:");
    for (const auto& dev : getDevices())
    {
        spdlog::debug("Device: {}", dev);
    }
    init();
}

SoundEngine::~SoundEngine()
{
    // all buffers should be unqueued and deleted before deleting the source
    // alSourcei(sourceID, AL_BUFFER, null);
    spdlog::debug("Destroying sound engine");
    if (ALCboolean contextMadeCurrent; !alcCallImpl(alcMakeContextCurrent, contextMadeCurrent, _device, nullptr))
    { /* what can you do? */
    }

    if (!alcCallImpl(alcDestroyContext, _device, _context))
    { /* not much you can do */
    }
    // we cannot use alcCallImpl here,since the device is no longer valid after closing, hence querying the error is not
    // possible
    if (!alcCloseDevice(_device)) { spdlog::error("Could not close device"); }
    //     if (ALCboolean closed; !alcCallImpl(alcCloseDevice, closed, _device, _device))
    //     { // do we care?
    //     }
    spdlog::debug("Sound engine destroyed");
}

void SoundEngine::init()
{
    _device = {alcOpenDevice(nullptr)};

    if (!alcCallImpl(alcCreateContext, _context, _device, _device, nullptr) || !_context)
    {
        /* probably exit program */
        throw std::runtime_error("Could not create audio context");
    }

    ALCboolean contextMadeCurrent = false;
    if (!alcCallImpl(alcMakeContextCurrent, contextMadeCurrent, _device, _context) || contextMadeCurrent != ALC_TRUE)
    {
        /* probably exit or give up on having sound */
        throw std::runtime_error("Could not make audio context current");
    }
}

std::vector<std::string> SoundEngine::getDevices() const
{
    const ALCchar* devices;
    if (!alcCallImpl(alcGetString, devices, nullptr, nullptr, ALC_DEVICE_SPECIFIER))
    {
        //
        return {};
    }
    return alStringToVector(devices, '\n');
}

std::vector<std::string> SoundEngine::getExtensions() const
{
    const ALCchar* alcExtension;
    if (!alcCallImpl(alcGetString, alcExtension, _device.device, nullptr, ALC_EXTENSIONS)) return {};
    const ALCchar* alExtension;
    if (!alcCallImpl(alGetString, alExtension, _device.device, AL_EXTENSIONS)) return {};

    auto alc_exts = alStringToVector(alcExtension, ' ');
    auto al_exts = alStringToVector(alExtension, ' ');
    // TODO:not yet implemented in gcc 14.1
    // alc_exts.append_range(al_exts);
    for (const auto& ext : al_exts)
    {
        alc_exts.push_back(ext);
    }
    return alc_exts;
}

template <auto F>
void forAllSources()
{
    // copy all sources to a new vector
    std::vector<ALuint> srcs{SourceManager::getSources().begin(), SourceManager::getSources().end()};
    alCallImpl(F, static_cast<ALsizei>(srcs.size()), srcs.data());
}

void SoundEngine::pauseAll()
{
    // pause all sources
    forAllSources<alSourcePausev>();
}

void SoundEngine::startAll()
{
    // pause all sources
    forAllSources<alSourcePlayv>();
}

void SoundEngine::stopAll()
{
    // pause all sources
    forAllSources<alSourceStopv>();
}
