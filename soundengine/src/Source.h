#pragma once
#include <AL/al.h>
#include <memory>

namespace soundEngineX {

class Buffer;

struct SourceConfiguration
{
    float pitch{1.0};
    float gain{1.0};
    bool  loop{false};
};

/**
 * Abstract sound source, with a audio buffer attached
 * Possibly later: Options for looping, pitch, volume, etc.
 * Thin wrapper around the source descriptor. Copies will share the buffer
 */

class Source
{
public:
    Source();
    Source(Source&&) = default;

    virtual ~Source();

    void                       attachBuffer(std::shared_ptr<Buffer> buffer);
    void                       play();
    void                       setSourceConfig(const SourceConfiguration& config);
    const SourceConfiguration& getSourceConfiguration() const;

    explicit operator ALuint() const { return source; };

    void applyConfiguration();

private:
    std::shared_ptr<Buffer> attachedBuffer;
    ALuint                  source{};
    SourceConfiguration     config;
};
} // namespace soundEngineX
