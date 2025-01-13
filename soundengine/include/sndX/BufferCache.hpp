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
    using BufferPtr = std::shared_ptr<soundEngineX::Buffer>;
    using StreamLoaderDelegate =
        std::function<BufferPtr(std::istream&, soundEngineX::loader::Type, soundEngineX::loader::LoadingCallback)>;
    using FileLoaderDelegate = std::function<BufferPtr(const std::string&, soundEngineX::loader::LoadingCallback)>;

    // TODO: this could be a std::span
    using MemBufLoaderDelegate = std::function<
        BufferPtr(const std::vector<char>&, soundEngineX::loader::Type, soundEngineX::loader::LoadingCallback)>;

    static bool has(const std::string& filename);

    static BufferPtr& get(const std::string&                    key,
                          FileLoaderDelegate                    file_loader_delegate,
                          soundEngineX::loader::LoadingCallback progress_cb = {});

    static BufferPtr& get(const std::string&                    key,
                          std::istream&                         stream,
                          const soundEngineX::loader::Type      type,
                          StreamLoaderDelegate                  stream_loader_delegate,
                          soundEngineX::loader::LoadingCallback progress_cb = {});

    static BufferPtr& get(const std::string&                    key,
                          const std::vector<char>&              data,
                          const soundEngineX::loader::Type      type,
                          MemBufLoaderDelegate                  loader_delegate,
                          soundEngineX::loader::LoadingCallback progress_cb = {});

    // non-delegating
    static BufferPtr& get(const std::string& key, soundEngineX::loader::LoadingCallback progress_cb = {});

    static BufferPtr& get(const std::string&                    key,
                          std::istream&                         stream,
                          const soundEngineX::loader::Type      type,
                          soundEngineX::loader::LoadingCallback progress_cb = {});

    static BufferPtr& get(const std::string&                    key,
                          const std::vector<char>&              data,
                          const soundEngineX::loader::Type      type,
                          soundEngineX::loader::LoadingCallback progress_cb = {});

protected:
    static inline std::unordered_map<std::string, BufferPtr> buffers{};
};

} // namespace soundEngineX