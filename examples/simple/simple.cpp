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

void simpleSync()

{
    // play two sounds in a row
    // the source will gain ownership of the buffer
    soundEngineX::Source(soundEngineX::loader::load("data/click.wav")).play();
    soundEngineX::Source(soundEngineX::loader::load("data/test.wav")).play();
    // wait for the sounds to finish
}

void simpleAsync()
{
    // play two sounds asynchronously
    // the source will gain ownership of the buffer
    auto source_a = soundEngineX::Source(soundEngineX::loader::load("data/mixkit-repeating-arcade-beep-1084.wav"));
    auto source_b = soundEngineX::Source(soundEngineX::loader::load("data/test.wav"));

    auto future_a = source_a.playAsync();
    auto future_b = source_b.playAsync();

    // wait for all futures to finish
    future_a.get();
    future_b.get();
}

void backgroundPlayer()
{
    BackgroundPlayer player;
    player.load("data/click.wav");
    player.load("data/test.wav");
    player.load("data/mixkit-repeating-arcade-beep-1084.wav");

    for (auto i = 0u; i < 1000; ++i)
    {
        // player.play("data/test.wav");
        player.play("data/mixkit-repeating-arcade-beep-1084.wav");
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        std::cout << "Playing sound " << i << "#running: " << player.getNumRunning() << "\n";
    }
    // todo wait properly
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    // initialize a SoundEngine instance
    soundEngineX::SoundEngine engine;
    // simpleSync();
    // simpleAsync();
    backgroundPlayer();
    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
catch (...)
{
    std::cerr << "Unknown exception" << std::endl;
}
