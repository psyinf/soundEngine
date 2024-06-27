#pragma once
#include <bit>
#include <cstdint>
#include <fstream>
#include <iostream>

#include <sndX/Buffer.hpp>

#include <cstring>

namespace soundEngineX::format {

soundEngineX::DataChunk load_wav(std::istream& in);

soundEngineX::DataChunk load_wav(std::string_view filename);
} // namespace soundEngineX::format
