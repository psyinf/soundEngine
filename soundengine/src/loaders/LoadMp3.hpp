#pragma once
#include <sndX/Buffer.hpp>
#include <iostream>

namespace soundEngineX::format {

soundEngineX::DataChunk load_mp3(std::istream& in);

soundEngineX::DataChunk load_mp3(std::string_view filename);
} // namespace soundEngineX::format
