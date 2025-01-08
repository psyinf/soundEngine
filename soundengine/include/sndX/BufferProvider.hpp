#pragma once
#include <sndX/Buffer.hpp>

namespace soundEngineX {
class BufferProvider
{
public:
    virtual bool                                   preLoad(const std::string& buffer_name) = 0;
    virtual bool                                   has(const std::string& buffer_name) = 0;
    virtual std::shared_ptr<soundEngineX::Buffer>& get(const std::string& buffer_name) = 0;
};

class BufferCacheProvider : public BufferProvider
{
public:
    bool preLoad(const std::string& buffer_name) override { return BufferCache::get(buffer_name) != nullptr; }

    bool has(const std::string& buffer_name) override { return BufferCache::has(buffer_name); }

    std::shared_ptr<soundEngineX::Buffer>& get(const std::string& buffer_name) override
    {
        return BufferCache::get(buffer_name);
    }
};

} // namespace soundEngineX
