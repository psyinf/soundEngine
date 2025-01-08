#pragma once
#include <sndX/Buffer.hpp>
#include <sndX/Source.hpp>
#include <sndX/Loader.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace soundEngineX {

class BufferCache
{
public:
    static bool has(const std::string& filename) { return buffers.find(filename) != buffers.end(); }

    static std::shared_ptr<soundEngineX::Buffer>& get(const std::string&                    filename,
                                                      soundEngineX::loader::LoadingCallback progress_cb = {})
    {
        auto it = buffers.find(filename);
        if (it == buffers.end())
        {
            buffers[filename] =
                std::make_unique<soundEngineX::Buffer>(soundEngineX::loader::load(filename, progress_cb));
        }
        return buffers[filename];
    }

protected:
    static inline std::unordered_map<std::string, std::shared_ptr<soundEngineX::Buffer>> buffers{};
};

} // namespace soundEngineX