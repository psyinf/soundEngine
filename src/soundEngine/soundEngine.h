#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <Buffer.h>
#include "Types.h"

#include <memory>

namespace soundEngineX {

class SoundEngine
{
public:
    SoundEngine();
    ~SoundEngine();
    std::shared_ptr<Buffer> getBuffer(const std::string& filename);

private:
    void init();
    void iterateDevices(Device &device);
     

    Device device;
    Context context{ nullptr };  
};

}// namespace soundEngineX