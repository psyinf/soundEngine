#pragma once
#include <functional>
#include <string>
#include <memory>

#include <sndX/Buffer.hpp>
#include <sndX/Loader.hpp>

namespace soundEngineX {
class Buffer;
using BufferPtr = std::shared_ptr<Buffer>;
using LoaderDelegate = std::function<BufferPtr(const std::string& key, soundEngineX::loader::LoadingCallback)>;

} // namespace soundEngineX