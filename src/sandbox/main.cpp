#include "Loader.h"
#include <Source.h>
#include <chrono>
#include <exception>
#include <fstream>
#include <future>
#include <iostream>
#include <soundEngine.h>
#include <thread>

//https://github.com/ErikMcClure/tinyoal/tree/main/TinyOAL
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
try
{
    soundEngineX::SoundEngine engine;
    soundEngineX::loader::Loader loader;

    auto source1 = soundEngineX::Source();
    auto source2 = soundEngineX::Source();
    std::shared_ptr<soundEngineX::Buffer> buffer2 =
    loader.load(std::ifstream("data/test.wav", std::ios::binary), soundEngineX::loader::Type::WAV);
    std::shared_ptr<soundEngineX::Buffer> buffer1 =
    loader.load(std::ifstream("data/test.wav", std::ios::binary), soundEngineX::loader::Type::WAV);

    source1.attachBuffer(buffer2);
    source2.attachBuffer(buffer1);

    source2.setSourceConfig({
      .pitch = 0.1,
      .gain = 1,
    });

    auto a = std::async(std::launch::async, [&source1]() { source1.play(); });
    auto a2 = std::async(std::launch::async, [&source2]() { source2.play(); });
    a.wait();
    a2.wait();


} catch (const std::exception &e)
{
    // TODO: Log exception
    std::cerr << e.what() << std::endl;
} catch (...)
{
    // TODO: Log unknown exception
    std::cerr << "Unknown exception" << std::endl;
}
