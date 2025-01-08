#pragma once
#include <sndX/BufferCache.hpp>
#include <pgf/taskengine/TaskEngine.hpp>
#include <sndX/BufferProvider.hpp>

namespace soundEngineX {

class BackgroundPlayer

{
public:
    using PlaybackFinishedCallback = std::function<void()>;
    BackgroundPlayer(std::shared_ptr<BufferProvider> buffer_provider = std::make_shared<BufferCacheProvider>());

    // pre-load a sound
    void preLoad(const std::string& buffer_name);
    // play a sound
    uint32_t play(const std::string& name, soundEngineX::SourceConfiguration&& cfg = {}, PlaybackFinishedCallback = {});

    // stop a sound
    void stop(uint32_t sourceId);

    void forceCheckPending();

    bool hasPendingTasks() const;

private:
    pg::foundation::TaskEngine      _taskEngine;
    std::shared_ptr<BufferProvider> _bufferProvider;
};

} // namespace soundEngineX