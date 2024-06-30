#include <sndX/Source.hpp>
#include <sndX/ALHelpers.hpp>
#include <sndX/Buffer.hpp>
#include <sndX/SourceManager.hpp>
#include <AL/al.h>
#include <AL/alc.h>

using namespace soundEngineX;

Source::Source()
{
    alCallImpl(alGenSources, 1, &_sourceId);
    SourceManager::addSource(_sourceId);
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
    SourceManager::removeSource(_sourceId);
}

void Source::play()
{
    applyConfiguration(_config);

    alCallImpl(alSourcePlay, _sourceId);
    if (_config.stream)
    {
        while (isPlaying())
        {
            fillStreamBuffers();
        }
    }
    else
    {
        ALint state = AL_PLAYING;
        while (state == AL_PLAYING)
        {
            // release some time to the CPU
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
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

void Source::applyConfiguration(const SourceConfiguration& config)
{
    alCallImpl(alSourcef, _sourceId, AL_PITCH, config.pitch);
    alCallImpl(alSourcef, _sourceId, AL_GAIN, config.gain);
    alCallImpl(alSourcei, _sourceId, AL_LOOPING, config.loop);
}

void Source::attachBuffer(std::shared_ptr<Buffer> buffer)
{
    _attachedBuffer = buffer;
    alCallImpl(alSourceQueueBuffers,
               _sourceId,
               static_cast<ALsizei>(buffer->getHandles().size()),
               buffer->getHandles().data());
}

void Source::stop()
{
    alCallImpl(alSourceStop, _sourceId);
}

void Source::start(const SourceConfiguration& config)
{
    applyConfiguration(config);
    alCallImpl(alSourcePlay, _sourceId);
}

void Source::start()
{
    start(_config);
}

void Source::pause()
{
    alCallImpl(alSourcePause, _sourceId);
}

void Source::fillStreamBuffers()
{
    ALint buffersProcessed = 0;
    alCallImpl(alGetSourcei, _sourceId, AL_BUFFERS_PROCESSED, &buffersProcessed);
    if (0 == buffersProcessed) { return; }
    std::vector<ALuint> buffers(buffersProcessed);
    alCallImpl(alSourceUnqueueBuffers, _sourceId, buffersProcessed, buffers.data());
    auto newBuffers = _attachedBuffer->buffersUnqueued(buffers);
    if (!newBuffers.empty())
    {
        alCallImpl(alSourceQueueBuffers, _sourceId, static_cast<ALsizei>(newBuffers.size()), newBuffers.data());
        alCallImpl(alSourcePlay, _sourceId);
    }
}

bool Source::isPlaying() const
{
    ALint state = AL_PLAYING;
    alCallImpl(alGetSourcei, _sourceId, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool Source::isStopped() const
{
    ALint state{};
    alCallImpl(alGetSourcei, _sourceId, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}

std::chrono::high_resolution_clock::duration Source::getDurationEstimation() const
{
    return _attachedBuffer->getDurationEstimation();
}
