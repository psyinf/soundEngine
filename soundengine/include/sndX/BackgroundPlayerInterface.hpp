#pragma once
#include <sndX/Buffer.hpp>
#include <sndX/Source.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace soundEngineX {

class BackgroundPlayerInterface
{
public:
    virtual ~BackgroundPlayerInterface() = default;

    virtual void load(const std::string& name) = 0;

    virtual uint32_t play(const std::string& name, soundEngineX::SourceConfiguration&& cfg = {}) = 0;

    virtual void stop(uint32_t sourceId) = 0;

protected:
    std::shared_ptr<soundEngineX::Buffer>&                                 getOrLoadBuffer(const std::string& filename);
    std::unordered_map<std::string, std::shared_ptr<soundEngineX::Buffer>> buffers;
};

} // namespace soundEngineX