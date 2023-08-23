#include <Source.h>
#include <chrono>
#include <exception>
#include <soundEngine.h>
#include <iostream>
#include <fstream>
#include <thread>
#include "Loader.h"


int main(int argc, char **argv)
try
{
    soundEngineX::SoundEngine se;
    soundEngineX::loader::Loader l;
    
    auto s1 = soundEngineX::Source();
    s1.attachBuffer(se.getBuffer("data/test.wav"));

    s1.attachBuffer(l.load(std::ifstream("data/test.wav"), soundEngineX::loader::Type::WAV));
   
    s1.setSourceConfig({
      .pitch = 1.0,
      .gain = 1,
    });
    
    
    s1.play();


} catch (const std::exception &e)
{
    // TODO: Log exception
    std::cerr << e.what() << std::endl;
} catch (...)
{   
    // TODO: Log unknown exception
    std::cerr << "Unkown exception" << std::endl;
}
