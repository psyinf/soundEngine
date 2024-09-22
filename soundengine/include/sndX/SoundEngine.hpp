#pragma once
#include <sndX/Types.hpp>
#include <atomic>
#include <string>
#include <vector>

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
    void init();

    Device  _device;
    Context _context{nullptr};
};

} // namespace soundEngineX
