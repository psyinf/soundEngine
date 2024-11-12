#include <sndX/Types.hpp>

#include <sndX/ALHelpers.hpp>
#include <sndX/SourceManager.hpp>
#include <stdexcept>

soundEngineX::Context::Context(std::shared_ptr<Device> device)
  : _device(device)
{
    if (!alcCallImpl(alcCreateContext, _context, device->get(), device->get(), nullptr) || !(_context))
    {
        throw std::runtime_error("Could not create audio context");
    }

    ALCboolean contextMadeCurrent = false;
    if (!alcCallImpl(alcMakeContextCurrent, contextMadeCurrent, device->get(), _context) ||
        contextMadeCurrent != ALC_TRUE)
    {
        throw std::runtime_error("Could not make audio context current");
    }
}

soundEngineX::Context::~Context()
{
    spdlog::debug("Destroying OpenAL context");
    ALCboolean contextMadeCurrent;
    alcCallImpl(alcMakeContextCurrent, contextMadeCurrent, _device->get(), nullptr);

    if (!alcCallImpl(alcDestroyContext, *_device, _context)) spdlog::debug("OpenAL context destroyed");
}

soundEngineX::Context::operator ALCcontext*()
{
    return _context;
}

soundEngineX::Context::operator ALCcontext const*()
{
    return _context;
}

bool soundEngineX::Context::operator!()
{
    return !_context;
}

soundEngineX::Device::Device(std::string_view device_name /*= ""*/)
  : _device(alcOpenDevice(device_name.data()))
{
}

soundEngineX::Device::~Device()
{
    if (_device)
    {
        if (!alcCloseDevice(_device)) { spdlog::error("Could not close device"); }
    }
}

soundEngineX::Device::operator ALCdevice*()
{
    return _device;
}
