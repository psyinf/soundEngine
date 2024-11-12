#include <sndX/SoundEngine.hpp>
#include <sndX/ALHelpers.hpp>
#include <sndX/SourceManager.hpp>

#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <ranges>
#include <string>

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
  : _device(Holder::getDevice())
  , _context(Holder::getContext())
{
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
    if (!alcCallImpl(alcGetString, alcExtension, _device->_device, nullptr, ALC_EXTENSIONS)) return {};
    const ALCchar* alExtension;
    if (!alcCallImpl(alGetString, alExtension, _device->_device, AL_EXTENSIONS)) return {};

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
    std::vector<ALuint> srcs = SourceManager::getSourcesIds();
    if (srcs.empty()) return;
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

SoundEngine::~SoundEngine()
{
    _context.reset();
    _device.reset();
    Holder::attempt_clean();
}
