#include <sndX/BackgroundPlayer.hpp>
#include <sndX/ALHelpers.hpp>

soundEngineX::BackgroundPlayer::BackgroundPlayer(std::shared_ptr<soundEngineX::BufferProvider> buffer_provider)
  : _taskEngine()
  , _bufferProvider(buffer_provider)
{
}

void soundEngineX::BackgroundPlayer::preLoad(const std::string& buffer_name)
{
    _bufferProvider->preLoad(buffer_name);
}

uint32_t soundEngineX::BackgroundPlayer::play(const std::string&                  name,
                                              soundEngineX::SourceConfiguration&& cfg /*= {}*/,
                                              PlaybackFinishedCallback            finished_callback /*={}*/)
{
    using namespace std::chrono_literals;
    auto       source = std::make_shared<soundEngineX::Source>(_bufferProvider->get(name), std::move(cfg));
    const auto duration = source->getDurationEstimation();

    _taskEngine.addTask([source]() { source->start(); });

    _taskEngine.addTask({.task =
                             [source, finished_callback]() {
                                 auto stopped = source->isStopped();
                                 if (stopped && finished_callback) { finished_callback(); }
                                 return stopped;
                             },
                         .reschedule_on_failure = true,
                         .starting_time_offset = duration,
                         .reschedule_delay = 100ms});
    return source->getSourceId();
}

void soundEngineX::BackgroundPlayer::stop(uint32_t sourceId)
{
    _taskEngine.addTask([sourceId]() { alCallImpl(alSourceStop, sourceId); });
}

void soundEngineX::BackgroundPlayer::forceCheckPending()
{
    _taskEngine.forceCheckTimedTasks();
}

bool soundEngineX::BackgroundPlayer::hasPendingTasks() const
{
    return !_taskEngine.hasTimedTasks();
}
