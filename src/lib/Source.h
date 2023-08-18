#pragma once
#include <AL/al.h>
#include <memory>
namespace soundEngineX {

class Buffer;
/**
 * Abstract sound source, with a audio buffer attached
 * Possibly later: Options for looping, pitch, volume, etc.
 */

struct SourceConfiguration
{
    float pitch{ 1.0 };
    float gain{ 1.0 };
    bool loop{ false };
};
class Source
{
public:
    Source();
    virtual ~Source();

    void attachBuffer(std::shared_ptr<Buffer> buffer);
    void play();
    void setSourceConfig(const SourceConfiguration &config);
    const SourceConfiguration &getSourceConfiguration() const; 
    operator ALuint() { return source; }

    void applyConfiguration();
    
private:
    std::shared_ptr<Buffer> attachedBuffer;
    ALuint source{};
    SourceConfiguration config;
};
}// namespace soundEngineX
