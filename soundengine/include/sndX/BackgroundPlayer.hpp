#pragma once
#include <sndX/BufferCache.hpp>
#include <sndX/TaskEngine.hpp>
#include <sndX/ALHelpers.hpp>
#include <spdlog/spdlog.h>

namespace soundEngineX {

class BackgroundPlayer
{
public:
    BackgroundPlayer()
      : taskEngine()
    {
    }

    // pre-load a buffer
    void load(const std::string& name) { BufferCache::get(name); }

    uint32_t play(const std::string& name, soundEngineX::SourceConfiguration&& cfg = {})
    {
        using namespace std::chrono_literals;
        auto       source = std::make_shared<soundEngineX::Source>(BufferCache::get(name), std::move(cfg));
        const auto duration = source->getDurationEstimation();

        taskEngine.addTask([source]() { source->start(); });

        taskEngine.addTask({.task = [source]() { return source->isStopped(); },
                            .reschedule_on_failure = true,
                            .starting_time_offset = duration,
                            .reschedule_delay = 100ms});
        return source->getSourceId();
    }

    void stop(uint32_t sourceId)
    {
        taskEngine.addTask([sourceId]() { alCallImpl(alSourceStop, sourceId); });
    }

    void forceCheckPending() { taskEngine.forceCheckTimedTasks(); }

private:
    soundEngineX::TaskEngine taskEngine;
};

} // namespace soundEngineX