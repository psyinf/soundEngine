#pragma once
#include <sndX/Buffer.hpp>
#include <sndX/Source.hpp>
#include <atomic>
#include <memory>

#include <sndX/BackgroundPlayerInterface.hpp>
#include <sndX/BackgroundPlayerExp.hpp>
#include <sndX/BackgroundPlayerThreaded.hpp>

namespace soundEngineX {
#ifdef EXPERIMENTAL_PLAYER
using BackgroundPlayer = BackgroundPlayerExp;
#else
using BackgroundPlayer = BackgroundPlayerThreaded;
#endif
} // namespace soundEngineX