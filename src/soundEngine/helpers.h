#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>

inline bool check_alc_errors(ALCdevice *device)
{
    const auto error = alcGetError(device);
    if (error != ALC_NO_ERROR) {
        // std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error) {
        case ALC_INVALID_VALUE:
            std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case ALC_INVALID_DEVICE:
            std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
            break;
        case ALC_INVALID_CONTEXT:
            std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
            break;
        case ALC_INVALID_ENUM:
            std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
            break;
        case ALC_OUT_OF_MEMORY:
            std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
            break;
        default:
            std::cerr << "UNKNOWN ALC ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}
inline bool check_al_errors()
{
    const auto error = alGetError();
    if (error != AL_NO_ERROR) {
        // std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error) {
        case AL_INVALID_NAME:
            std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
            break;
        case AL_INVALID_ENUM:
            std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
            break;
        case AL_INVALID_VALUE:
            std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case AL_INVALID_OPERATION:
            std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
            break;
        case AL_OUT_OF_MEMORY:
            std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            break;
        default:
            std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

template<typename alcFunction, typename... Params>
inline auto alcCallImpl(alcFunction function, ALCdevice *device, Params&&... params) ->
  typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
    function(std::forward<Params>(params)...);
    return check_alc_errors(device);
}

template<typename alcFunction, typename ReturnType, typename... Params>
inline auto alcCallImpl(alcFunction function, ReturnType &returnValue, ALCdevice *device, Params&&... params) ->
  typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool>
{
    returnValue = function(std::forward<Params>(params)...);
    return check_alc_errors(device);
}

template<typename alFunction, typename... Params>
inline auto alCallImpl(alFunction function, Params&&... params) ->
  typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
{
    auto ret = function(std::forward<Params>(params)...);
    check_al_errors();
    return ret;
}

template<typename alFunction, typename... Params>
inline auto alCallImpl(alFunction function, Params&&... params) ->
  typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
    function(std::forward<Params>(params)...);
    return check_al_errors();
}