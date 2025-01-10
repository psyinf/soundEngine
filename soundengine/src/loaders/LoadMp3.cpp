#include "LoadMp3.hpp"
#define MINIMP3_IMPLEMENTATION

#include <sndX/SoundEngine.hpp>
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

struct ProgressCBInfo
{
    soundEngineX::loader::LoadingCallback  progress_cb;
    std::string                            filename;
    soundEngineX::loader::LoadProgressInfo lastProgress;
};

MP3D_PROGRESS_CB makeCallbackWrapper()
{
    return +[](void* user_data, size_t file_size, uint64_t offset, mp3dec_frame_info_t*) -> int {
        auto& [cb, filename, lastProgress] = *reinterpret_cast<ProgressCBInfo*>(user_data);
        // only issue if progress has changed by more than 1%
        if (cb.cb && (offset - lastProgress.loaded_size) > (file_size / 100))
        {
            lastProgress = {offset, file_size};
            cb.cb({offset, file_size});
        }
        return 0;
    };
}

soundEngineX::DataChunk soundEngineX::format::load_mp3(std::istream&                         in,
                                                       soundEngineX::loader::LoadingCallback progress_cb)
{
    mp3dec_t           dec{};
    mp3dec_file_info_t mp3Info{};
    mp3dec_init(&dec);
    auto           guard = nonstd::make_scope_exit([&mp3Info] {
        // there is seemingly no mp3dec_free/close
        ::free(static_cast<void*>(mp3Info.buffer));
    });
    ProgressCBInfo cb_info{progress_cb, "filename"};
    int            ret;

    std::vector<char> buffer(std::istreambuf_iterator<char>{in}, {});
    mp3dec_map_info_t map_info;
    map_info.buffer = reinterpret_cast<const uint8_t*>(buffer.data());
    map_info.size = buffer.size();

    mp3CallImpl(
        mp3dec_load_mapinfo, &dec, &map_info, &mp3Info, progress_cb.cb ? makeCallbackWrapper() : nullptr, &cb_info);

    spdlog::debug("Freq {} Hz, {} Kbits/s", mp3Info.hz, mp3Info.avg_bitrate_kbps);

    soundEngineX::FormatDescriptor formatDesc;
    formatDesc.channels = static_cast<std::uint8_t>(mp3Info.channels);
    formatDesc.bitsPerSample = 8 * sizeof(mp3d_sample_t);
    formatDesc.sampleRateHz = mp3Info.hz;

    return {formatDesc,
            std::vector<char>(reinterpret_cast<char*>(mp3Info.buffer),
                              reinterpret_cast<char*>(mp3Info.buffer + mp3Info.samples))};
}

soundEngineX::DataChunk soundEngineX::format::load_mp3(std::string_view                      filename,
                                                       soundEngineX::loader::LoadingCallback progress_cb)
{
    mp3dec_t           dec{};
    mp3dec_file_info_t mp3Info{};
    mp3dec_init(&dec);
    auto           guard = nonstd::make_scope_exit([&mp3Info] {
        // there is seemingly no mp3dec_free/close
        ::free(static_cast<void*>(mp3Info.buffer));
    });
    ProgressCBInfo cb_info{progress_cb, "filename"};

    mp3CallImpl(
        mp3dec_load, &dec, filename.data(), &mp3Info, progress_cb.cb ? makeCallbackWrapper() : nullptr, &cb_info);

    spdlog::debug("Freq {} Hz, {} Kbits/s", mp3Info.hz, mp3Info.avg_bitrate_kbps);

    soundEngineX::FormatDescriptor formatDesc;
    formatDesc.channels = static_cast<std::uint8_t>(mp3Info.channels);
    formatDesc.bitsPerSample = 8 * sizeof(mp3d_sample_t);
    formatDesc.sampleRateHz = mp3Info.hz;

    return {formatDesc,
            std::vector<char>(reinterpret_cast<char*>(mp3Info.buffer),
                              reinterpret_cast<char*>(mp3Info.buffer + mp3Info.samples))};
}
