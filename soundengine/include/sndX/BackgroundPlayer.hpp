#pragma once
#include <sndX/Buffer.hpp>
#include <sndX/Source.hpp>
#include <atomic>
#include <memory>

#include <sndX/BackgroundPlayerInterface.hpp>
#include <sndX/BackgroundPlayerTasked.hpp>
#include <sndX/BackgroundPlayerThreaded.hpp>

namespace soundEngineX {
#ifdef FALLBACK_PLAYER
using BackgroundPlayer = BackgroundPlayerThreaded;
#else
using BackgroundPlayer = BackgroundPlayerTasked;
#endif
} // namespace soundEngineX