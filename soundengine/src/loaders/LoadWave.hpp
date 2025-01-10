#pragma once
#include <bit>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sndX/Loader.hpp>
#include <sndX/Buffer.hpp>

#include <cstring>

namespace soundEngineX::format {

soundEngineX::DataChunk load_wav(std::istream& in, soundEngineX::loader::LoadingCallback progress_cb);

soundEngineX::DataChunk load_wav(std::string_view filename, soundEngineX::loader::LoadingCallback progress_cb);

soundEngineX::DataChunk load_wav(const std::vector<char>& buffer, soundEngineX::loader::LoadingCallback progress_cb);
} // namespace soundEngineX::format
