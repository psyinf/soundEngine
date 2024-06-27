#include <Loader.hpp>
#include <SoundEngine.hpp>
#include <Source.hpp>
#include <BackgroundPlayer.hpp>
#include <chrono>
#include <exception>
#include <iostream>
#include <thread>

void simpleSync()
{
    // play two sounds in a row
    // the source will gain ownership of the buffer
    soundEngineX::Source(soundEngineX::loader::load("data/demo/click.wav")).play();
    soundEngineX::Source(soundEngineX::loader::load("data/demo/test.wav")).play();
    // wait for the sounds to finish
}

void simpleAsync()
{
    // play two sounds asynchronously
    // the source will gain ownership of the buffer
    auto source_a = soundEngineX::Source(soundEngineX::loader::load("data/demo/mixkit-repeating-arcade-beep-1084.wav"));
    auto source_b = soundEngineX::Source(soundEngineX::loader::load("data/demo/test.wav"));

    auto future_a = source_a.playAsync();
    auto future_b = source_b.playAsync();

    // wait for all futures to finish
    future_a.get();
    future_b.get();
}

void backgroundPlayer()
{
    soundEngineX::BackgroundPlayer player;
    player.load("data/demo/click.wav");
    player.load("data/demo/test.wav");
    player.load("data/demo/mixkit-repeating-arcade-beep-1084.wav");

    for (auto i = 0u; i < 100; ++i)
    {
        // play a sound every 100ms
        auto handle = player.play("data/demo/mixkit-repeating-arcade-beep-1084.wav", {.gain = 0.05f});
        if (i % 10 == 0) { player.play("data/demo/test.wav"); }
        std::this_thread::sleep_for(std::chrono::milliseconds(110));
        // player.stop(handle);
    }
    // todo wait properly
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    // initialize a SoundEngine instance
    soundEngineX::SoundEngine engine;
    engine.getExtensions();

    simpleSync();
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
