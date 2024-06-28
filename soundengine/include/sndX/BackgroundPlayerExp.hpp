#pragma once
#include <sndX/BackgroundPlayer.hpp>
#include <spdlog/spdlog.h>

namespace soundEngineX {
class BackgroundPlayerExp : public BackgroundPlayerInterface
{
public:
    virtual ~BackgroundPlayerExp()
    {
        while (true)
        {
            if (const std::lock_guard<std::mutex> lock(mutex); sources.empty()) { break; }
            // spin lock waiting for all sources to finish
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(100ms);
        }
    }

    void load(const std::string& name) { getOrLoadBuffer(name); }

    uint32_t play(const std::string& name, soundEngineX::SourceConfiguration&& cfg = {})
    {
        auto init = std::chrono::high_resolution_clock::now();
        auto buffer = getOrLoadBuffer(name);
        auto source = std::make_shared<soundEngineX::Source>(buffer, std::move(cfg));
        // async, store future
        {
            const std::lock_guard<std::mutex> lock(mutex);

            sources[source->getSourceId()] = std::async(std::launch::async | std::launch::deferred, [this, init, source]() {
                //                 spdlog::info("register: {}",
                //                              std::chrono::duration_cast<std::chrono::microseconds>(
                //                                  std::chrono::high_resolution_clock::now() - init)
                //                                  .count());
                source->play();
                remove(source->getSourceId());
            });
        }

        return source->getSourceId();
    }

    void stop(uint32_t sourceId)
    {
        // TODO: stop the source
        // auto it = sources.find(sourceId);
        // if (it != sources.end()) { it->second->stop(); }
    }

    void remove(uint32_t sourceId)
    {
        std::future<void> future;

        {
            const std::lock_guard<std::mutex> lock(mutex);
            auto                              it = sources.find(sourceId);
            // move out of map
            if (it != sources.end())
            {
                future = std::move(it->second);
                sources.erase(it);
            }
        }
        future.get();
    }

private:
    std::unordered_map<uint32_t, std::future<void>> sources;
    std::mutex                                      mutex;
};

} // namespace soundEngineX