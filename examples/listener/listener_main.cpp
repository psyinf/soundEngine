#include <sndX/BackgroundPlayer.hpp>
#include <sndX/Loader.hpp>
#include <sndX/SoundEngine.hpp>
#include <sndX/Source.hpp>

#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include <sndX/Listener.hpp>

void backgroundPlayer()
{
    soundEngineX::BackgroundPlayer player;
    pg::foundation::TaskEngine     taskEngine;
    player.load("data/demo/mixkit-repeating-arcade-beep-1084.wav");
    // add some task that modifies the listener gain
    taskEngine.addTask({.task{[]() {
                            auto gain = soundEngineX::Listener::getGain();
                            std::this_thread::sleep_for(std::chrono::milliseconds(30));
                            soundEngineX::Listener::setGain(gain + 0.1f);
                            if (gain > 5.0f) { soundEngineX::Listener::setGain(0.0f); }

                            return false;
                        }},
                        .reschedule_on_failure{true}});

    for (auto i = 0u; i < 3; ++i)
    {
        // start to play a sound every 2000ms
        auto handle = player.play("data/demo/mixkit-repeating-arcade-beep-1084.wav", {.gain = 0.05f});
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        // stop the sound
        player.stop(handle);
    }
    // todo wait properly
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    soundEngineX::SoundEngine engine;
    //
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
