#include <sndX/Loader.hpp>
#include "loaders/LoadWave.hpp"
#include "loaders/LoadMp3.hpp"
#include <filesystem>

namespace soundEngineX::loader {

DataDescriptor load(std::string_view name, LoadingCallback progress_cb)
{
    auto type = getType(name);
    // return load(, type);
    return load(name, type, progress_cb);
}

DataDescriptor load(std::string_view name, Type type, LoadingCallback progress_cb)
{
    switch (type)
    {
    case Type::WAV: {
        auto&& chunk = soundEngineX::format::load_wav(name, progress_cb);
        return DataDescriptor{{chunk}};
    }
    case Type::MP3: {
        auto&& chunk = soundEngineX::format::load_mp3(name, progress_cb);
        return DataDescriptor{{chunk}};
    }
    break;
    default:
        throw std::invalid_argument("Unknown type");
        break;
    }
}

DataDescriptor load(std::istream& stream, Type type, LoadingCallback progress_cb)
{
    switch (type)
    {
    case Type::WAV: {
        auto&& chunk = soundEngineX::format::load_wav(stream, progress_cb);
        return DataDescriptor{{chunk}};
    }
    case Type::MP3: {
        auto&& chunk = soundEngineX::format::load_mp3(stream, progress_cb);
        return DataDescriptor{{chunk}};
    }
    break;
    default:
        throw std::invalid_argument("Unknown type");
        break;
    }
}

soundEngineX::DataDescriptor load(const std::vector<char>& data, Type type, LoadingCallback progress_cb)
{
    switch (type)
    {
    case Type::WAV: {
        auto&& chunk = soundEngineX::format::load_wav(data, progress_cb);
        return DataDescriptor{{chunk}};
    }
    case Type::MP3: {
        auto&& chunk = soundEngineX::format::load_mp3(data, progress_cb);
        return DataDescriptor{{chunk}};
    }
    break;
    default:
        throw std::invalid_argument("Unknown type");
        break;
    }
}

DataDescriptor loadMultiple(const std::vector<std::string>& names, soundEngineX::loader::LoadingCallback progress_cb)
{
    DataDescriptor dataDescriptor{};
    for (const auto& name : names)
    {
        auto loadedDescriptor = load(name, progress_cb);
        dataDescriptor.chunks.insert(
            dataDescriptor.chunks.end(), loadedDescriptor.chunks.begin(), loadedDescriptor.chunks.end());
    }
    return dataDescriptor;
}

Type getType(std::string_view full_name)
{
    auto extension = std::filesystem::path(full_name).extension().string();

    // in case the extension is empty, we assume the name is only the extension
    extension = extension.empty() ? full_name : extension;
    std::transform(extension.begin(), extension.end(), extension.begin(), [](auto c) {
        return static_cast<char>(std::tolower(c));
    });
    // remove the dot, if there is one
    if (extension.front() == '.') { extension = extension.substr(1); }

    if (extension == "wav") { return Type::WAV; }
    else if (extension == "mp3") { return Type::MP3; }
    else { throw std::invalid_argument("Cannot determine file type from extension"); }
}

} // namespace soundEngineX::loader