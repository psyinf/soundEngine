#pragma once
#
#include <AL/alc.h>

#include <string_view>
#include <memory>

namespace soundEngineX {
struct Device
{
    Device(std::string_view device_name = "");

    ~Device();
    Device(Device const&) = delete;

    ALCdevice* _device = nullptr;

    operator ALCdevice*();

    ALCdevice* get() { return _device; }
};

struct Context
{
    Context(std::shared_ptr<Device> device);

    ~Context();

    operator ALCcontext*();

    operator ALCcontext const*();

    bool operator!();

    ALCcontext* _context{nullptr};

    std::shared_ptr<Device> _device;
};

struct Holder
{
    static void attempt_clean()
    {
        if (_shared_context.use_count() == 1)
        {
            _shared_device.reset();
            _shared_context.reset();
        }
    }

    static std::shared_ptr<Device> getDevice()
    {
        if (!_shared_device) { _shared_device = std::make_shared<Device>(""); }

        return _shared_device;
    }

    static std::shared_ptr<Context> getContext()
    {
        if (!_shared_context) { _shared_context = std::make_shared<Context>(getDevice()); }

        return _shared_context;
    }

    static inline std::shared_ptr<Device>  _shared_device{};
    static inline std::shared_ptr<Context> _shared_context{nullptr};
};
} // namespace soundEngineX