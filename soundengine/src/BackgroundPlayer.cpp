#include <sndX/BackgroundPlayer.hpp>

 soundEngineX::BackgroundPlayer::BackgroundPlayer()
  : taskEngine()
{
}

void soundEngineX::BackgroundPlayer::load(const std::string& name)
{
    BufferCache::get(name);
}

uint32_t soundEngineX::BackgroundPlayer::play(const std::string& name, soundEngineX::SourceConfiguration&& cfg /*= {}*/)
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

void soundEngineX::BackgroundPlayer::stop(uint32_t sourceId)
{
    taskEngine.addTask([sourceId]() { alCallImpl(alSourceStop, sourceId); });
}

void soundEngineX::BackgroundPlayer::forceCheckPending()
{
    taskEngine.forceCheckTimedTasks();
}

bool soundEngineX::BackgroundPlayer::hasPendingTasks() const
{
    return !taskEngine.hasTimedTasks();
}
