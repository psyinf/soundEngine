#pragma once
#include <sndX/BackgroundPlayerInterface.hpp>
#include <sndX/TaskEngine.hpp>
#include <sndX/ALHelpers.hpp>
#include <spdlog/spdlog.h>

namespace soundEngineX {

class BackgroundPlayerTasked : public soundEngineX::BackgroundPlayerInterface
{
public:
    BackgroundPlayerTasked()
      : soundEngineX::BackgroundPlayerInterface()
      , taskEngine()
    {
    }

    void load(const std::string& name) override { getOrLoadBuffer(name); }

    uint32_t play(const std::string& name, soundEngineX::SourceConfiguration&& cfg = {}) override
    {
        using namespace std::chrono_literals;
        auto source = std::make_shared<soundEngineX::Source>(getOrLoadBuffer(name), std::move(cfg));
        auto duration = source->getDurationEstimation();

        taskEngine.addTask([source]() { source->start(); });

        taskEngine.addTask({.task = [source]() { return source->isStopped(); },
                            .reschedule_on_failure = true,
                            .starting_time_offset = duration,
                            .reschedule_delay = 100ms});
        return source->getSourceId();
    }

    void stop(uint32_t sourceId) override { alCallImpl(alSourceStop, sourceId); }

private:
    soundEngineX::TaskEngine taskEngine;
};

} // namespace soundEngineX