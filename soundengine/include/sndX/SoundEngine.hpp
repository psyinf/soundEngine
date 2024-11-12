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
    void init();

    std::shared_ptr<Device>  _device;
    std::shared_ptr<Context> _context;

    static inline std::shared_ptr<Device>  _shared_device{};
    static inline std::shared_ptr<Context> _shared_context{nullptr};
};

} // namespace soundEngineX
