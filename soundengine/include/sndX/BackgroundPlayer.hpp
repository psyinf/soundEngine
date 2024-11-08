#pragma once
#include <sndX/BufferCache.hpp>
#include <pgf/taskengine/TaskEngine.hpp>
#include <sndX/ALHelpers.hpp>

namespace soundEngineX {

class BackgroundPlayer
{
public:
    using PlaybackFinishedCallback = std::function<void()>;
    BackgroundPlayer();

    // pre-load a buffer
    void load(const std::string& name);

    // play a sound
    uint32_t play(const std::string& name, soundEngineX::SourceConfiguration&& cfg = {}, PlaybackFinishedCallback = {});

    // stop a sound
    void stop(uint32_t sourceId);

    void forceCheckPending();

    bool hasPendingTasks() const;

private:
    pg::foundation::TaskEngine taskEngine;
};

} // namespace soundEngineX