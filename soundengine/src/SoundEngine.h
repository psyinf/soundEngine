#pragma once

#include "Types.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <Buffer.h>
#include <string>

#include <memory>

namespace soundEngineX {

class SoundEngine
{
public:
    SoundEngine();
    ~SoundEngine();

private:
    void init();
    void iterateDevices(Device& device);

    Device  device;
    Context context{nullptr};
};

} // namespace soundEngineX