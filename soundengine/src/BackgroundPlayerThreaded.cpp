#include <sndX/BackgroundPlayerThreaded.hpp>
#include <sndX/Loader.hpp>
#include <spdlog/spdlog.h>

soundEngineX::BackgroundPlayerThreaded::~BackgroundPlayerThreaded()
{
    {
        const std::lock_guard<std::mutex> lock(mutex);
        // std::ranges::for_each(sources, [](auto& source) { source.second->stop(); });
        for (auto& source : sources)
        {
            source.second->stop();
        };
    }
    // wait for all threads to finish
    auto still_running = numRunning.load();
    while (still_running)
    {
        spdlog::info("Waiting for {} source threads to finish", still_running);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        still_running = numRunning.load();
    }
}

void soundEngineX::BackgroundPlayerThreaded::load(const std::string& name)
{
    // make sure the the buffer exists
    getOrLoadBuffer(name);
}

uint32_t soundEngineX::BackgroundPlayerThreaded::play(const std::string& name, soundEngineX::SourceConfiguration&& cfg)
{
    // TODO: starting the thread is taking almost 1m, so maybe we use a thread pool or some async mechanism
    auto init = std::chrono::high_resolution_clock::now();
    auto buffer = getOrLoadBuffer(name);

    std::shared_ptr<soundEngineX::Source> source = std::make_shared<soundEngineX::Source>(buffer, cfg);
    {
        std::lock_guard<std::mutex> lock(mutex);
        sources[source->getSourceId()] = source;
    }
    std::thread([this, source, init]() mutable {
        numRunning++;
        spdlog::info(
            "register: {}",
            std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - init)
                .count());
        source->play();

        remove(source->getSourceId());
        numRunning--;
    }).detach();
    return source->getSourceId();
}

void soundEngineX::BackgroundPlayerThreaded::stop(uint32_t sourceId)
{
    const std::lock_guard<std::mutex> lock(mutex);
    auto                              it = sources.find(sourceId);
    if (it != sources.end())
    {
        it->second->stop();
        sources.erase(it);
    }
}

void soundEngineX::BackgroundPlayerThreaded::remove(uint32_t sourceId)
{
    const std::lock_guard<std::mutex> lock(mutex);
    auto                              it = sources.find(sourceId);
    if (it != sources.end()) { sources.erase(it); }
}

std::shared_ptr<soundEngineX::Buffer>& soundEngineX::BackgroundPlayerInterface::getOrLoadBuffer(
    const std::string& filename)
{
    auto it = buffers.find(filename);
    if (it == buffers.end())
    {
        buffers[filename] = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(filename));
    }
    return buffers[filename];
}
