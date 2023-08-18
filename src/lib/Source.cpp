#include "Source.h"
#include "Buffer.h"
#include "helpers.h"
#include <AL/al.h>
#include <AL/alc.h>
using namespace soundEngineX;

Source::Source()
{
    alCallImpl(alGenSources, 1, &source);
}

Source::~Source()
{
    alCallImpl(alDeleteSources, 1, &source);
}

void Source::play()
{
    alCallImpl(alSourcePlay, source);
}


void Source::setSourceConfig(const SourceConfiguration &config)
{
    this->config = config;
}

const SourceConfiguration &Source::getSourceConfiguration() const
{
    return config;
}

void Source::applyConfiguration()
{
    alCallImpl(alSourcef, source, AL_PITCH, config.pitch);
    alCallImpl(alSourcef, source, AL_GAIN, config.gain);
    alCallImpl(alSourcei, source, AL_LOOPING, config.loop);
}

void Source::attachBuffer(std::shared_ptr<Buffer> buffer)
{
    attachedBuffer = buffer;
    alCallImpl(alSourcei, source, AL_BUFFER, *buffer);
}
