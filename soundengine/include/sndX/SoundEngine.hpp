#pragma once
#include <sndX/Types.hpp>
#include <string>
#include <vector>
#include <memory>

namespace soundEngineX {
class SoundEngine
{
public:
    SoundEngine();

    ~SoundEngine();

    std::vector<std::string> getDevices() const;
    std::vector<std::string> getExtensions() const;

    void pauseAll();
    void startAll();
    void stopAll();

private:
    std::shared_ptr<Device>  _device;
    std::shared_ptr<Context> _context;
};

} // namespace soundEngineX
