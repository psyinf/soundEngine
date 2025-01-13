#include <sndX/BufferCache.hpp>

bool soundEngineX::BufferCache::has(const std::string& filename)
{
    return buffers.find(filename) != buffers.end();
}

soundEngineX::BufferPtr& soundEngineX::BufferCache::get(const std::string&                    key,
                                                        const std::vector<char>&              data,
                                                        const soundEngineX::loader::Type      type,
                                                        soundEngineX::loader::LoadingCallback progress_cb /*= {}*/)
{
    if (!buffers.contains(key))
    {
        buffers[key] = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(data, type, progress_cb));
    }
    return buffers[key];
}

soundEngineX::BufferPtr& soundEngineX::BufferCache::get(const std::string&                    key,
                                                        LoaderDelegate&                       file_loader_delegate,
                                                        soundEngineX::loader::LoadingCallback progress_cb /*= {}*/)
{
    if (!buffers.contains(key))
    {
        if (file_loader_delegate) { buffers[key] = file_loader_delegate(key, progress_cb); }
        else { buffers[key] = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(key, progress_cb)); }
    }
    return buffers[key];
}

soundEngineX::BufferPtr& soundEngineX::BufferCache::get(const std::string&                    key,
                                                        soundEngineX::loader::LoadingCallback progress_cb /*= {}*/)
{
    if (!buffers.contains(key))
    {
        buffers[key] = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(key, progress_cb));
    }
    return buffers[key];
}

soundEngineX::BufferPtr& soundEngineX::BufferCache::get(const std::string&                    key,
                                                        std::istream&                         stream,
                                                        const soundEngineX::loader::Type      type,
                                                        soundEngineX::loader::LoadingCallback progress_cb /*= {}*/)
{
    if (!buffers.contains(key))
    {
        buffers[key] = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(stream, type, progress_cb));
    }
    return buffers[key];
}

soundEngineX::BufferPtr soundEngineX::BufferCache::retrieve(const std::string& key)
{
    return buffers.at(key);
}