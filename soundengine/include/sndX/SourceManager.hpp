
#pragma once
#include <unordered_set>
#include <sndX/Source.hpp>

namespace soundEngineX {

class SourceManager
{
public:
    static void addSource(soundEngineX::Source::SourceHandle source) { sources.insert(source); }

    static void removeSource(soundEngineX::Source::SourceHandle source) { sources.erase(source); }

    static const std::unordered_set<soundEngineX::Source::SourceHandle>& getSources() { return sources; }

    static bool hasSource(soundEngineX::Source::SourceHandle source) { return sources.find(source) != sources.end(); }

private:
    inline static std::unordered_set<soundEngineX::Source::SourceHandle> sources{};
};

} // namespace soundEngineX