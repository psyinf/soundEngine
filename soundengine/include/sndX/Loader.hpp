#pragma once
#include <sndX/Buffer.hpp>
#include <vector>
#include <string>
#include <functional>

namespace soundEngineX::loader {
struct LoadProgressInfo
{
    uint64_t loaded_size{};
    size_t   complete_size{};

    float percent() const { return static_cast<float>(loaded_size) / static_cast<float>(complete_size); }
};

struct LoadingCallback
{
    std::string                           resource;
    std::function<void(LoadProgressInfo)> cb;
};
enum class Type
{
    WAV,
    MP3,
};

DataDescriptor load(std::string_view name, LoadingCallback = {});
DataDescriptor load(std::string_view name, Type type, LoadingCallback = {});
DataDescriptor load(std::istream& stream, Type type, LoadingCallback = {});
DataDescriptor loadMultiple(const std::vector<std::string>& names, LoadingCallback = {});

Type getType(std::string_view name);

} // namespace soundEngineX::loader