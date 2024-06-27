#include <Loader.hpp>
#include <loaders/LoadWave.hpp>
#include <loaders/LoadMp3.hpp>
#include <filesystem>

namespace soundEngineX::loader {

DataDescriptor load(std::string_view name)
{
    auto type = getType(name);
    // return load(, type);
    return load(name, type);
}

DataDescriptor load(std::string_view name, Type type)
{
    switch (type)
    {
    case Type::WAV: {
        auto&& chunk = soundEngineX::format::load_wav(name);
        return DataDescriptor{{chunk}};
    }
    case Type::MP3: {
        auto&& chunk = soundEngineX::format::load_mp3(name);
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
    std::transform(extension.begin(), extension.end(), extension.begin(), [](auto c) {
        return static_cast<char>(std::tolower(c));
    });
    if (extension == ".wav") { return Type::WAV; }
    else if (extension == ".mp3") { return Type::MP3; }
    else { throw std::invalid_argument("Cannot determine file type from extension"); }
}

} // namespace soundEngineX::loader