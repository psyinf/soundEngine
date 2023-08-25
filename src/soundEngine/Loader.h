#pragma once
#include <istream>
#include <vector>
#include "Buffer.h"
namespace soundEngineX::loader {


enum class Type {
    WAV,
};
class Loader
{
public:
    Loader() = default;

    std::unique_ptr<soundEngineX::Buffer> load(std::istream &&stream, Type type);
    std::unique_ptr<soundEngineX::Buffer> loadStream(std::istream &stream, Type type);
    
};


}// namespace soundEngineX::loader