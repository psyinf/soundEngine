// #include <pgfoundation/taskengine.hpp>
#include <sndX/BackgroundPlayer.hpp>
#include <sndX/Loader.hpp>
#include <sndX/SoundEngine.hpp>
#include <sndX/Source.hpp>
#include <sndX/ALHelpers.hpp>
#include <chrono>
#include <exception>
#include <iostream>
#include <thread>

void taskedPlayerTest()
{
    soundEngineX::BackgroundPlayer player;
    player.play("data/demo/click.wav");
    player.play("data/demo/test.wav");
    player.play("data/demo/mixkit-repeating-arcade-beep-1084.wav");
}

void taskedEngineTest()
{
    pg::foundation::TaskEngine player;
    using namespace std::chrono_literals;
    auto now = std::chrono::high_resolution_clock::now();
    int  state = 0;
    std::cout << "Starting tasks\n";
    // current time in seconds
    auto t =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    std::cout << t << ": starting\n";
    player.addTask({.task =
                        [&state]() {
                            state++;
                            // current time in seconds
                            auto t = std::chrono::duration_cast<std::chrono::milliseconds>(
                                         std::chrono::high_resolution_clock::now().time_since_epoch())
                                         .count();
                            std::cout << t << ": delayed run#" << state << "\n ";
                            return state > 3;
                        },
                    .reschedule_on_failure = true,
                    .starting_time_offset = 2s,
                    .reschedule_delay = 1s}

    );
    player.wait();
    player.stop();
}

void taskedPlayerSoundTest()
{
    pg::foundation::TaskEngine player;
    auto src1 = std::make_shared<soundEngineX::Source>(soundEngineX::loader::load("data/demo/click.wav"));
    auto src2 = std::make_shared<soundEngineX::Source>(soundEngineX::loader::load("data/demo/test.wav"));

    player.addTask(pg::foundation::Task{.task = [&src1]() {
        src1->start();
        return "";
    }});
    // player.addTask([]() { std::this_thread::sleep_for(std::chrono::seconds(3)); });

    player.addTask([&src2]() { src2->start(); });
    player.addTask([&src1]() { src1->start(); });
    player.addTask([]() { std::this_thread::sleep_for(std::chrono::seconds(3)); });
    player.wait();
    player.stop();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    soundEngineX::SoundEngine soundEngine;
    // initialize a SoundEngine instance
    taskedPlayerTest();
    // taskedPlayerSoundTest();
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
