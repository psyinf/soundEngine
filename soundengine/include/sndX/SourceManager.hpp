
#pragma once
#include <unordered_set>
#include <sndX/Source.hpp>

namespace soundEngineX {

class SourceManager
{
public:
    static void addSource(soundEngineX::Source* source) { sources.insert(source); }

    static void removeSource(soundEngineX::Source* source) { sources.erase(source); }

    static const std::vector<soundEngineX::Source::SourceHandle> getSourcesIds()
    {
        std::vector<soundEngineX::Source::SourceHandle> ret(sources.size());
        std::transform(
            sources.begin(), sources.end(), ret.begin(), [](soundEngineX::Source* s) { return s->getSourceId(); });
        return ret;
    }

    // static void clear() { sources.clear(); }

private:
    inline static std::unordered_set<soundEngineX::Source*> sources{};
};

} // namespace soundEngineX