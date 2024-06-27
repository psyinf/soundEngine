#include "LoadMp3.hpp"
#define MINIMP3_IMPLEMENTATION

#include <SoundEngine.hpp>
#include <spdlog/spdlog.h>
#include <nonstd/scope.hpp>
// disable warnings
#pragma warning(push, 0)
#include "minimp3_ex.h"
#pragma warning(pop)

template <typename mp3decFunc, typename... Params>
inline void mp3CallImpl(mp3decFunc function, Params&&... params)
{
    auto ret_code = function(std::forward<Params>(params)...);
    if (ret_code < 0)
    {
        switch (ret_code)
        {
        case MP3D_E_PARAM:
            throw std::invalid_argument("MP3D_E_PARAM: invalid parameter");
            break;
        case MP3D_E_MEMORY:
            throw std::runtime_error("MP3D_E_MEMORY: memory error");
            break;
        case MP3D_E_IOERROR:
            throw std::runtime_error("MP3D_E_IO: IO error");
            break;
        case MP3D_E_USER:
            throw std::runtime_error("MP3D_E_USER: user error");
            break;
        case MP3D_E_DECODE:
            throw std::runtime_error("MP3D_E_DECODE: decode error");
            break;
        default:
            throw std::runtime_error("MP3D: Unknown error " + std::to_string(ret_code));
            break;
        }
    }
}

soundEngineX::DataChunk soundEngineX::format::load_mp3(std::istream& /*in*/)
{
    throw std::runtime_error("Not yet implemented");
}

soundEngineX::DataChunk soundEngineX::format::load_mp3(std::string_view filename)
{
    mp3dec_t           dec;
    mp3dec_file_info_t mp3Info;
    mp3dec_init(&dec);
    auto guard = nonstd::make_scope_exit([&mp3Info] {
        // there is seamingly no mp3dec_free/close
        ::free(static_cast<void*>(mp3Info.buffer));
    });
    mp3CallImpl(mp3dec_load, &dec, filename.data(), &mp3Info, nullptr, nullptr);

    spdlog::debug("Freq {} Hz, {} Kbits/s", mp3Info.hz, mp3Info.avg_bitrate_kbps);

    soundEngineX::FormatDescriptor formatDesc;
    formatDesc.channels = static_cast<std::uint8_t>(mp3Info.channels);
    formatDesc.bitsPerSample = 8 * sizeof(mp3d_sample_t);
    formatDesc.sampleRateHz = mp3Info.hz;

    return {formatDesc,
            std::vector<char>(reinterpret_cast<char*>(mp3Info.buffer),
                              reinterpret_cast<char*>(mp3Info.buffer + mp3Info.samples))};
}
