#pragma once
#include <sndX/BufferLoaderDelegate.hpp>
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
    static bool has(const std::string& filename);

    /*Get the buffer with the given key. If not in cache the supplied file_loader_delegate will be called with the key
     * to fill the buffer-cache*/
    static BufferPtr& get(const std::string&                    key,
                          LoaderDelegate&                       file_loader_delegate,
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
    // only getting the buffer
    static BufferPtr retrieve(const std::string& key);

protected:
    static inline std::unordered_map<std::string, BufferPtr> buffers{};
};

} // namespace soundEngineX