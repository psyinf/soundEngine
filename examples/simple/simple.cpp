#include <sndX/BackgroundPlayer.hpp>
#include <sndX/Loader.hpp>
#include <sndX/SoundEngine.hpp>
#include <sndX/Source.hpp>

#include <chrono>
#include <exception>
#include <iostream>
#include <thread>

#include <fstream>

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

    auto source_a = soundEngineX::Source(soundEngineX::loader::load("data/demo/mixkit-repeating-arcade-beep-1084.wav"));
    auto source_b = soundEngineX::Source(soundEngineX::loader::load("data/demo/test.wav"));

    source_a.start({});
    source_b.start({});

    auto future_a = std::async(std::launch::async, [&source_a]() {
        while (source_a.isPlaying())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        };
    });
    auto future_b = std::async(std::launch::async, [&source_b]() {
        while (source_b.isPlaying())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        };
    });

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

    for (auto i = 0u; i < 3; ++i)
    {
        // play a sound every 1000ms
        auto handle = player.play("data/demo/mixkit-repeating-arcade-beep-1084.wav", {.gain = 0.05f});
        if (i % 10 == 0) { player.play("data/demo/test.wav"); }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // stop the sound
        player.stop(handle);
    }
    // todo wait properly
}

void testCallback()
{
    soundEngineX::loader::load(
        R"(e:\develop\projects\resources\galaxyResources\data\music\a-meditation-through-time-amp-space-11947.mp3)",
        {"space.mp3", [](const soundEngineX::loader::LoadProgressInfo& load_info) {
             std::cout << load_info.loaded_size << std::endl;
         }});

    soundEngineX::loader::load(R"(e:\develop\projects\resources\soundResources\data\demo\the-stargazer-112474.wav)",
                               {"space.mp3", [](const soundEngineX::loader::LoadProgressInfo& load_info) {
                                    std::cout << load_info.loaded_size << std::endl;
                                }});
}

void testLoader()
{
    auto type = soundEngineX::loader::getType("Mp3");
    // test stream loading MP3
    std::ifstream file(
        R"(e:\develop\projects\resources\galaxyResources\data\music\a-meditation-through-time-amp-space-11947.mp3)",
        std::ios::binary);
    auto res = soundEngineX::loader::load(file, type, {"", [](const soundEngineX::loader::LoadProgressInfo& load_info) {
                                                           std::cout << load_info.loaded_size << std::endl;
                                                       }});

    soundEngineX::Source(res).play();
}
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    // initialize a SoundEngine instance
    soundEngineX::SoundEngine engine;
    engine.getExtensions();
    // test loading callback
    testLoader();
    simpleSync();
    simpleAsync();
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
