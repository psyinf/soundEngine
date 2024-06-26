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
#include <BackgroundPlayer.h>

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

    for (auto i = 0u; i < 1000; ++i)
    {
        // player.play("data/demo/test.wav");
        player.play("data/demo/mixkit-repeating-arcade-beep-1084.wav");
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
