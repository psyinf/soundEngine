#pragma once
#include <Buffer.h>
#include <Source.h>
#include <atomic>
#include <memory>

namespace soundEngineX {

// naive background player
class BackgroundPlayer
{
public:
    ~BackgroundPlayer();

    using SourcePtr = std::shared_ptr<soundEngineX::Source>;

    void load(const std::string& name);

    uint32_t play(const std::string& name, soundEngineX::SourceConfiguration&& cfg = {});

    void stop(uint32_t sourceId);

private:
    void remove(uint32_t sourceId);

    std::shared_ptr<soundEngineX::Buffer>& getOrLoadBuffer(const std::string& filename);

    std::unordered_map<uint32_t, SourcePtr>                                sources;
    std::unordered_map<std::string, std::shared_ptr<soundEngineX::Buffer>> buffers;
    std::mutex                                                             mutex;
    std::atomic<uint32_t>                                                  numRunning;
};

} // namespace soundEngineX