#include <sndX/Source.hpp>
#include <sndX/ALHelpers.hpp>
#include <sndX/Buffer.hpp>
#include <AL/al.h>
#include <AL/alc.h>

using namespace soundEngineX;

Source::Source()
{
    alCallImpl(alGenSources, 1, &_sourceId);
}

Source::Source(std::shared_ptr<Buffer>& buffer)
  : Source()
{
    attachBuffer(buffer);
}

Source::Source(Buffer&& buffer)
  : Source()
{
    attachBuffer(std::make_unique<Buffer>(std::move(buffer)));
}

Source::Source(std::shared_ptr<Buffer>& buffer, const SourceConfiguration& cfg)
  : Source()
{
    attachBuffer(buffer);
    setSourceConfig(cfg);
}

Source::~Source()
{
    if (_sourceId) { alCallImpl(alDeleteSources, 1, &_sourceId); }
}

void Source::play()
{
    applyConfiguration();

    alCallImpl(alSourcePlay, _sourceId);
    if (_config.stream)
    {
        for (ALint state = AL_PLAYING; state == AL_PLAYING;)
        {
            ALint buffersProcessed = 0;
            alCallImpl(alGetSourcei, _sourceId, AL_SOURCE_STATE, &state);
            alCallImpl(alGetSourcei, _sourceId, AL_BUFFERS_PROCESSED, &buffersProcessed);
            if (0 == buffersProcessed) { continue; }
            std::vector<ALuint> buffers(buffersProcessed);
            alCallImpl(alSourceUnqueueBuffers, _sourceId, buffersProcessed, buffers.data());
            auto newBuffers = _attachedBuffer->buffersUnqueued(buffers);
            if (!newBuffers.empty())
            {
                alCallImpl(alSourceQueueBuffers, _sourceId, static_cast<ALsizei>(newBuffers.size()), newBuffers.data());
                alCallImpl(alSourcePlay, _sourceId);
            }
        }
    }
    else
    {
        ALint state = AL_PLAYING;
        while (state == AL_PLAYING)
        {
            alCallImpl(alGetSourcei, _sourceId, AL_SOURCE_STATE, &state);
        }
    }
}

void Source::setSourceConfig(const SourceConfiguration& config)
{
    this->_config = config;
}

const SourceConfiguration& Source::getSourceConfiguration() const
{
    return _config;
}

void Source::applyConfiguration()
{
    alCallImpl(alSourcef, _sourceId, AL_PITCH, _config.pitch);
    alCallImpl(alSourcef, _sourceId, AL_GAIN, _config.gain);
    alCallImpl(alSourcei, _sourceId, AL_LOOPING, _config.loop);
}

void Source::attachBuffer(std::shared_ptr<Buffer> buffer)
{
    _attachedBuffer = buffer;
    alCallImpl(alSourceQueueBuffers,
               _sourceId,
               static_cast<ALsizei>(buffer->getHandles().size()),
               buffer->getHandles().data());
}

std::future<void> Source::playAsync()
{
    return std::async(std::launch::async, [&]() { play(); });
}

void Source::stop()
{
    alCallImpl(alSourceStop, _sourceId);
}
