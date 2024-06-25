#pragma once
#include <bit>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>
#include <SoundEngine.h>

#include <cstring>

namespace soundEngineX::format {

soundEngineX::DataChunk load_wav(std::istream& in);

soundEngineX::DataChunk load_wav(const std::string& filename);
} // namespace soundEngineX::format
