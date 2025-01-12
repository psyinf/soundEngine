#pragma once
#include <sndX/Buffer.hpp>
#include <sndX/Source.hpp>
#include <sndX/Loader.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace soundEngineX {

/* Mono-state that allows to "get" resources based on a filename, stream or memory buffer.
 * The cache will load the resource if it is not already loaded.
 * The cache will use the provided delegates to load the resource if they are set.
 * If the delegates are not set, the cache will use the default loader.
 */

class BufferCache
{
public:
    using StreamLoaderDelegate =
        std::function<std::shared_ptr<soundEngineX::Buffer>(std::istream&,
                                                            soundEngineX::loader::Type,
                                                            soundEngineX::loader::LoadingCallback)>;
    using FileLoaderDelegate =
        std::function<std::shared_ptr<soundEngineX::Buffer>(const std::string&, soundEngineX::loader::LoadingCallback)>;

    // TODO: this should be a std::span
    using MemBufLoaderDelegate =
        std::function<std::shared_ptr<soundEngineX::Buffer>(const std::vector<char>&,
                                                            soundEngineX::loader::Type,
                                                            soundEngineX::loader::LoadingCallback)>;

    static bool has(const std::string& filename) { return buffers.find(filename) != buffers.end(); }

    static std::shared_ptr<soundEngineX::Buffer>& get(const std::string&                    key,
                                                      FileLoaderDelegate                    file_loader_delegate = {},
                                                      soundEngineX::loader::LoadingCallback progress_cb = {})
    {
        if (!buffers.contains(key))
        {
            if (file_loader_delegate) { buffers[key] = file_loader_delegate(key, progress_cb); }
            else
            {
                buffers[key] = std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(key, progress_cb));
            }
        }
        return buffers[key];
    }

    static std::shared_ptr<soundEngineX::Buffer>& get(const std::string&                    key,
                                                      std::istream&                         stream,
                                                      const soundEngineX::loader::Type      type,
                                                      StreamLoaderDelegate                  stream_loader_delegate = {},
                                                      soundEngineX::loader::LoadingCallback progress_cb = {})
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

    static std::shared_ptr<soundEngineX::Buffer>& get(const std::string&                    key,
                                                      const std::vector<char>&              data,
                                                      const soundEngineX::loader::Type      type,
                                                      MemBufLoaderDelegate                  loader_delegate = {},
                                                      soundEngineX::loader::LoadingCallback progress_cb = {})
    {
        if (!buffers.contains(key))
        {
            if (loader_delegate) { buffers[key] = loader_delegate(data, type, progress_cb); }
            else
            {
                buffers[key] =
                    std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(data, type, progress_cb));
            }
        }
        return buffers[key];
    }

protected:
    static inline std::unordered_map<std::string, std::shared_ptr<soundEngineX::Buffer>> buffers{};
};

} // namespace soundEngineX