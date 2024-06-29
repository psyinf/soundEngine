#include <sndX/TaskEngine.hpp>
#include <sndX/BackgroundPlayerInterface.hpp>
#include <sndX/Loader.hpp>
#include <sndX/SoundEngine.hpp>
#include <sndX/Source.hpp>
#include <sndX/AlHelpers.hpp>
#include <chrono>
#include <exception>
#include <iostream>
#include <thread>

class BackgroundPlayerTasked : public soundEngineX::BackgroundPlayerInterface
{
public:
    BackgroundPlayerTasked()
      : soundEngineX::BackgroundPlayerInterface()
      , taskEngine()
    {
    }

    void load(const std::string& name) override { getOrLoadBuffer(name); }

    uint32_t play(const std::string& name, soundEngineX::SourceConfiguration&& cfg = {}) override
    {
        using namespace std::chrono_literals;
        auto source = std::make_shared<soundEngineX::Source>(getOrLoadBuffer(name), std::move(cfg));
        auto duration = source->getDurationEstimation();

        taskEngine.addTask([source]() { source->start(); });

        taskEngine.addTask({.task = [source]() { return source->isStopped(); },
                            .reschedule_on_failure = true,
                            .starting_time_offset = duration,
                            .reschedule_delay = 100ms});
        return source->getSourceId();
    }

    void stop(uint32_t sourceId) override { alCallImpl(alSourceStop, sourceId); }

private:
    soundEngineX::TaskEngine taskEngine;
};

void taskedPlayerTest()
{
    BackgroundPlayerTasked player;
    player.play("data/demo/click.wav");
    player.play("data/demo/test.wav");
    player.play("data/demo/mixkit-repeating-arcade-beep-1084.wav");
}

void taskedEngineTest()
{
    soundEngineX::TaskEngine player;
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
    soundEngineX::TaskEngine player;
    auto src1 = std::make_shared<soundEngineX::Source>(soundEngineX::loader::load("data/demo/click.wav"));
    auto src2 = std::make_shared<soundEngineX::Source>(soundEngineX::loader::load("data/demo/test.wav"));

    player.addTask(soundEngineX::Task{.task = [&src1]() {
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
