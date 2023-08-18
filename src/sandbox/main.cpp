#include <Source.h>
#include <chrono>
#include <exception>
#include <soundEngine.h>
#include <iostream>
#include <thread>


int main(int argc, char **argv)
try
{
    soundEngineX::SoundEngine se;

    auto s1 = soundEngineX::Source();
    s1.attachBuffer(se.getBuffer("data/test.wav"));
    // s1.applyConfiguration();
    s1.play();
    std::this_thread::sleep_for(std::chrono::seconds(3));


} catch (const std::exception &e)
{
    // TODO: Log exception
    std::cerr << e.what() << std::endl;
} catch (...)
{
    // TODO: Log unknown exception
}
