#include "Source.h"
#include "Buffer.h"
#include "ALHelpers.h"
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
    applyConfiguration();
   
    alCallImpl(alSourcePlay, source);
    for (ALint state = AL_PLAYING; state == AL_PLAYING;)
    {
        ALint buffersProcessed = 0;
        alCallImpl(alGetSourcei, source, AL_SOURCE_STATE, &state);
        alCallImpl(alGetSourcei, source, AL_BUFFERS_PROCESSED, &buffersProcessed);
        if (0 == buffersProcessed)
        {
            continue;
        }
        std::vector<ALuint> buffers(buffersProcessed);
        alCallImpl(alSourceUnqueueBuffers, source, buffersProcessed, buffers.data());
        auto newBuffers = attachedBuffer->buffersUnqueued(buffers);
        if (!newBuffers.empty())
        {
            alCallImpl(alSourceQueueBuffers, source, static_cast<ALsizei>(newBuffers.size()), newBuffers.data());
            alCallImpl(alSourcePlay, source);
        }
    }
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

void Source::attachBuffer(std::shared_ptr<Buffer> &buffer)
{
    attachedBuffer = buffer;
    alCallImpl(
      alSourceQueueBuffers, source, static_cast<ALsizei>(buffer->getHandles().size()), buffer->getHandles().data());
}
