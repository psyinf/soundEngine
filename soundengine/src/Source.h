#pragma once
#include <AL/al.h>
#include <memory>
#include <future>

namespace soundEngineX {

class Buffer;

struct SourceConfiguration
{
    float pitch{1.0};
    float gain{1.0};
    bool  loop{false};
    bool  stream{false};

    // monadic builder
    SourceConfiguration& withPitch(float p)
    {
        pitch = p;
        return *this;
    }

    SourceConfiguration& withGain(float g)
    {
        gain = g;
        return *this;
    }

    SourceConfiguration& setLoop(bool l)
    {
        loop = l;
        return *this;
    }

    SourceConfiguration& setStreaming(bool s)
    {
        stream = s;
        return *this;
    }
};

/**
 * Abstract sound source, with a audio buffer attached
 * Possibly later: Options for looping, pitch, volume, etc.
 * Thin wrapper around the source descriptor. Copies will share the buffer
 */

class Source
{
public:
    using SourceHandle = uint32_t;
    Source();
    virtual ~Source();

    Source(std::shared_ptr<Buffer>&);
    Source(std::shared_ptr<Buffer>&, const SourceConfiguration&);
    Source(Buffer&&);

    void                       attachBuffer(std::shared_ptr<Buffer> buffer);
    void                       play();
    void                       stop();
    std::future<void>          playAsync();
    void                       setSourceConfig(const SourceConfiguration& config);
    const SourceConfiguration& getSourceConfiguration() const;

    uint32_t getSourceId() const { return _sourceId; };

    explicit operator ALuint() const { return _sourceId; };

    void applyConfiguration();

private:
    std::shared_ptr<Buffer> _attachedBuffer;
    ALuint                  _sourceId{};
    SourceConfiguration     _config;
};

inline bool operator==(const Source& lhs, const Source& rhs)
{
    return lhs.getSourceId() == rhs.getSourceId();
}
} // namespace soundEngineX
