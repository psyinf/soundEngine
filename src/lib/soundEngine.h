#pragma once

#include <AL/al.h>
#include <AL/alc.h>


namespace soundEngineX {

struct Device
{
    ALCdevice *device = nullptr;

    operator ALCdevice *() { return device; }
};


struct Context
{
    Context(ALCcontext *c) : context(c) {}

    ALCcontext *context = nullptr;

    operator ALCcontext *() { return context; }
    operator ALCcontext const *() { return context; }

    bool operator!() { return !context; }
};

class SoundEngine
{
public:
    SoundEngine();
    ~SoundEngine();

private:
    void init();
    void iterateDevices(Device &device);

    Device device;
    Context context{ nullptr };
};

}// namespace soundEngineX