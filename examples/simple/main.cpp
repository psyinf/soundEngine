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

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    // initialize a SoundEngine instance
    soundEngineX::SoundEngine engine;
    // build a buffer from an example file
    auto buffer = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load("data/click.wav"));
    auto source = soundEngineX::Source();
    source.attachBuffer(std::move(buffer));

    source.play();

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
