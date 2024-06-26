#include "Loader.h"
#include <SoundEngine.h>
#include <Source.h>
#include <chrono>
#include <coroutine>
#include <exception>
#include <fstream>
#include <future>
#include <iostream>
#include <ranges>
#include <thread>

void sharedBuffer()
{
    soundEngineX::SoundEngine engine;

    auto                                  source1 = soundEngineX::Source();
    auto                                  source2 = soundEngineX::Source();
    std::shared_ptr<soundEngineX::Buffer> buffer1 =
        std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load("data/demo/test.wav"));
    std::shared_ptr<soundEngineX::Buffer> buffer2 =
        std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load("data/demo/test.wav"));

    source1.attachBuffer(buffer1);
    source2.attachBuffer(buffer1);

    source2.setSourceConfig({
        .pitch = 0.5,
        .gain = 1,
    });

    auto a = std::async(std::launch::async, [&source2]() { source2.play(); });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto a2 = std::async(std::launch::async, [&source1]() { source1.play(); });
    a.wait();
    a2.wait();
}

void queuedBuffer()
{
    soundEngineX::SoundEngine engine;

    auto source = soundEngineX::Source();

    std::shared_ptr<soundEngineX::Buffer> buffer = std::make_unique<soundEngineX::Buffer>(
        soundEngineX::loader::loadMultiple({"data/demo/click.wav", "data/demo/test.wav"}));

    source.attachBuffer(buffer);

    source.setSourceConfig({
        .pitch = 3.5,
        .gain = 1,
    });

    auto a = std::async(std::launch::async, [&source]() { source.play(); });
    std::this_thread::sleep_for(std::chrono::seconds(1));

    a.wait();
}

struct SimpleGenerator
{
    SimpleGenerator(std::vector<std::string>&& set, size_t times)
      : names(set)
      , makesLeft(times * names.size())
    {
    }

    size_t getMakesLeft() const { return makesLeft; }

    const std::string& make() const
    {
        if (0 == getMakesLeft()) { throw std::runtime_error("No more makes left"); }
        index %= names.size();
        const auto& ret = names.at(index);
        ++index;
        --makesLeft;
        return ret;
    }

    std::vector<std::string> names;
    mutable size_t           index{};
    mutable size_t           makesLeft{};
};

void queuedBufferRepeat()
{
    soundEngineX::SoundEngine engine;
    auto                      gen = SimpleGenerator({"data/demo/click.wav",
                                                     "data/demo/click.wav",
                                                     "data/demo/click.wav",
                                                     "data/demo/stop.wav",
                                                     "data/demo/stop.wav",
                                                     "data/demo/stop.wav"},
                               3);
    auto                      source = soundEngineX::Source();

    std::shared_ptr<soundEngineX::Buffer> buffer = std::make_unique<soundEngineX::Buffer>(
        soundEngineX::loader::loadMultiple({"data/demo/click.wav", "data/demo/test.wav"}));
    buffer->setRequestNewDataCallback([&gen](auto size_to_load) { //
        auto files = std::vector<std::string>(std::min(size_to_load, gen.getMakesLeft()));
        std::generate(files.begin(), files.end(), [&gen]() { return gen.make(); });
        return soundEngineX::loader::loadMultiple(files);
    });

    source.attachBuffer(buffer);

    source.setSourceConfig({
        .pitch = 3.5,
        .gain = 1,
    });

    auto a = std::async(std::launch::async, [&source]() { source.play(); });
    std::this_thread::sleep_for(std::chrono::seconds(1));

    a.wait();
}
// https://github.com/ErikMcClure/tinyoal/tree/main/TinyOAL
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    queuedBufferRepeat();
    return 0;
}
catch (const std::exception& e)
{
    // TODO: Log exception
    std::cerr << e.what() << std::endl;
}
catch (...)
{
    // TODO: Log unknown exception
    std::cerr << "Unknown exception" << std::endl;
}
