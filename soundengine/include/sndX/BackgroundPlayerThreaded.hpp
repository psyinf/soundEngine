#pragma once

#include <sndX/BackgroundPlayerInterface.hpp>

namespace soundEngineX {
// naive background player
class BackgroundPlayerThreaded : public BackgroundPlayerInterface
{
public:
    virtual ~BackgroundPlayerThreaded();

    using SourcePtr = std::shared_ptr<soundEngineX::Source>;

    void load(const std::string& name);

    uint32_t play(const std::string& name, soundEngineX::SourceConfiguration&& cfg = {});

    void stop(uint32_t sourceId);

private:
    void remove(uint32_t sourceId);

    std::unordered_map<uint32_t, SourcePtr> sources;

    std::mutex            mutex;
    std::atomic<uint32_t> numRunning;
};

} // namespace soundEngineX