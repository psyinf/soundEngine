#pragma once
#include "Buffer.h"
#include <istream>
#include <vector>

namespace soundEngineX::loader {

enum class Type
{
    WAV,
    MP3,
};

DataDescriptor load(std::string_view name);
DataDescriptor load(std::string_view name, Type type);
DataDescriptor loadMultiple(const std::vector<std::string>& names);

Type getType(std::string_view name);

} // namespace soundEngineX::loader