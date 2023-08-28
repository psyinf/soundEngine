#include "Loader.h"
#include "loadWave.h"
#include <filesystem>
#include <istream>
namespace soundEngineX::loader {

DataDescriptor Loader::load(std::string_view name)
{
    auto type = getType(name);
    return load(std::ifstream(name.data(), std::ios::binary), type);
}
DataDescriptor Loader::load(std::istream &&stream, Type type)
{
    switch (type)
    {
    case Type::WAV: {

        auto &&[data, format] = load_wav(stream);
        return DataDescriptor{ .chunks = { { .format = format, .data = data } } };
    }
    break;
    default:
        throw std::invalid_argument("Unknown type");
        break;
    }
}

DataDescriptor Loader::loadMultiple(const std::vector<std::string> &names)
{
    DataDescriptor dataDescriptor{};
    for (const auto &name : names)
    {
        auto loadedDescriptor = load(name);
        dataDescriptor.chunks.insert(
          dataDescriptor.chunks.end(), loadedDescriptor.chunks.begin(), loadedDescriptor.chunks.end());
    }
    return dataDescriptor;
}


soundEngineX::loader::Type Loader::getType(std::string_view name) const
{
    auto extension = std::filesystem::path(name).extension().string();
    std::transform(
      extension.begin(), extension.end(), extension.begin(), [](auto c) { return std::tolower(c); });
    if (extension == ".wav")
    {
        return Type::WAV;
    } else
    {
        throw std::invalid_argument("Cannot determine file type from extension");
    }
}

}// namespace soundEngineX::loader