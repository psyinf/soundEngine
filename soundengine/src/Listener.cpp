#include <sndX/Listener.hpp>


#include <sndX/ALHelpers.hpp>

float soundEngineX::Listener::getGain() 
{
    ALfloat gain;
    alCallImpl(alGetListenerf, AL_GAIN, &gain);
    return gain;
}

void soundEngineX::Listener::setGain(float gain)
{
    alCallImpl(alListenerf, AL_GAIN, gain);
}
