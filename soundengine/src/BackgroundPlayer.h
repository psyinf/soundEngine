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
    ~BackgroundPlayer()
    {
        while (running > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    using SourcePtr = std::shared_ptr<soundEngineX::Source>;

    void load(const std::string& name)
    {
        // make sure the the buffer exists
        getOrLoadBuffer(name);
    }

    void play(const std::string& name)
    {
        auto buffer = getOrLoadBuffer(name);
        // play the sound asynchronously and detach the source
        std::thread([this, &buffer]() mutable {
            soundEngineX::Source source(buffer);
            running++;
            source.play();
            running--;
            // TODO hold weak_ptr to source to keep track or stop it
        }).detach();
    }

    auto getNumRunning() const { return running.load(); }

private:
    std::shared_ptr<soundEngineX::Buffer>& getOrLoadBuffer(const std::string& filename)
    {
        auto it = buffers.find(filename);
        if (it == buffers.end())
        {
            buffers[filename] = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(filename));
        }
        return buffers[filename];
    }

    std::unordered_map<std::string, std::shared_ptr<soundEngineX::Buffer>> buffers;
    std::atomic_uint64_t                                                   running{0};
};

} // namespace soundEngineX