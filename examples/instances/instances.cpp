#include <iostream>
#include <sndX/Loader.hpp>
#include <sndX/SoundEngine.hpp>
#include <sndX/Source.hpp>
#include <sndX/Types.hpp>
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    soundEngineX::SoundEngine engine;
    soundEngineX::SoundEngine engine2;

    auto source = new soundEngineX::Source(soundEngineX::loader::load("data/demo/click.wav"));

    source->play();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

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