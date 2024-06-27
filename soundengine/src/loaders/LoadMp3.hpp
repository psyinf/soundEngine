#pragma once
#include <iostream>
#include <string>
#include <SoundEngine.hpp>
#include <Buffer.hpp>

namespace soundEngineX::format {

soundEngineX::DataChunk load_mp3(std::istream& in);

soundEngineX::DataChunk load_mp3(std::string_view filename);
} // namespace soundEngineX::format
