#pragma once
#include <sndX/Buffer.hpp>

namespace soundEngineX {
class BufferProvider
{
public:
    virtual bool load(const std::string& buffer_name, LoaderDelegate delegate = {}) = 0;
    virtual bool has(const std::string& buffer_name) = 0;
    virtual std::shared_ptr<soundEngineX::Buffer>& get(const std::string& buffer_name,
                                                       LoaderDelegate     delegate = {}) = 0;
};

class BufferCacheProvider : public BufferProvider
{
public:
    /** load the buffer with the given key. If not in cache the supplied file_loader_delegate will be called with the
     * key to fill the buffer-cache. If no delegate is provided the buffer_name is interpreted as file name
     **/
    bool load(const std::string& buffer_name, LoaderDelegate delegate = {}) override
    {
        return BufferCache::get(buffer_name, delegate) != nullptr;
    }

    bool has(const std::string& buffer_name) override { return BufferCache::has(buffer_name); }

    std::shared_ptr<soundEngineX::Buffer>& get(const std::string& buffer_name, LoaderDelegate delegate = {}) override
    {
        return BufferCache::get(buffer_name, delegate);
    }
};

} // namespace soundEngineX
