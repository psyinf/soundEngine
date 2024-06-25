#include "Loader.h"
#include "LoadWave.h"
#include <filesystem>
#include <istream>

namespace soundEngineX::loader {

DataDescriptor load(std::string_view name)
{
    auto type = getType(name);
    return load(std::ifstream(name.data(), std::ios::binary), type);
}

DataDescriptor load(std::istream&& stream, Type type)
{
    switch (type)
    {
    case Type::WAV: {
        auto&& chunk = soundEngineX::format::load_wav(stream);
        return DataDescriptor{{chunk}};
    }
    break;
    default:
        throw std::invalid_argument("Unknown type");
        break;
    }
}

DataDescriptor loadMultiple(const std::vector<std::string>& names)
{
    DataDescriptor dataDescriptor{};
    for (const auto& name : names)
    {
        auto loadedDescriptor = load(name);
        dataDescriptor.chunks.insert(
            dataDescriptor.chunks.end(), loadedDescriptor.chunks.begin(), loadedDescriptor.chunks.end());
    }
    return dataDescriptor;
}

Type getType(std::string_view name)
{
    auto extension = std::filesystem::path(name).extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), [](auto c) { return std::tolower(c); });
    if (extension == ".wav") { return Type::WAV; }
    else { throw std::invalid_argument("Cannot determine file type from extension"); }
}

} // namespace soundEngineX::loader