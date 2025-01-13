#include <sndX/BufferCache.hpp>

bool soundEngineX::BufferCache::has(const std::string& filename)
{
    return buffers.find(filename) != buffers.end();
}

soundEngineX::BufferCache::BufferPtr& soundEngineX::BufferCache::get(
    const std::string&                    key,
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

soundEngineX::BufferCache::BufferPtr& soundEngineX::BufferCache::get(
    const std::string&                    key,
    FileLoaderDelegate                    file_loader_delegate,
    soundEngineX::loader::LoadingCallback progress_cb /*= {}*/)
{
    if (!buffers.contains(key))
    {
        if (file_loader_delegate) { buffers[key] = file_loader_delegate(key, progress_cb); }
        else { buffers[key] = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(key, progress_cb)); }
    }
    return buffers[key];
}

soundEngineX::BufferCache::BufferPtr& soundEngineX::BufferCache::get(
    const std::string&                    key,
    std::istream&                         stream,
    const soundEngineX::loader::Type      type,
    StreamLoaderDelegate                  stream_loader_delegate,
    soundEngineX::loader::LoadingCallback progress_cb /*= {}*/)
{
    if (!buffers.contains(key))
    {
        if (stream_loader_delegate) { buffers[key] = stream_loader_delegate(stream, type, progress_cb); }
        else
        {
            buffers[key] =
                std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(stream, type, progress_cb));
        }
    }
    return buffers[key];
}

soundEngineX::BufferCache::BufferPtr& soundEngineX::BufferCache::get(
    const std::string&                    key,
    const std::vector<char>&              data,
    const soundEngineX::loader::Type      type,
    MemBufLoaderDelegate                  loader_delegate /*= {}*/,
    soundEngineX::loader::LoadingCallback progress_cb /*= {}*/)
{
    if (!buffers.contains(key))
    {
        if (loader_delegate) { buffers[key] = loader_delegate(data, type, progress_cb); }
        else
        {
            buffers[key] = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(data, type, progress_cb));
        }
    }
    return buffers[key];
}

soundEngineX::BufferCache::BufferPtr& soundEngineX::BufferCache::get(
    const std::string&                    key,
    soundEngineX::loader::LoadingCallback progress_cb /*= {}*/)
{
    if (!buffers.contains(key))
    {
        buffers[key] = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(key, progress_cb));
    }
    return buffers[key];
}

soundEngineX::BufferCache::BufferPtr& soundEngineX::BufferCache::get(
    const std::string&                    key,
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
