#pragma once
#include <sndX/Buffer.hpp>
#include <sndX/Loader.hpp>
#include <iostream>

namespace soundEngineX::format {

soundEngineX::DataChunk load_mp3(std::istream& in, soundEngineX::loader::LoadingCallback progress_cb);

soundEngineX::DataChunk load_mp3(std::string_view filename, soundEngineX::loader::LoadingCallback progress_cb);
} // namespace soundEngineX::format
