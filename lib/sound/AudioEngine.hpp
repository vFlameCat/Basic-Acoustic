#pragma once


#include <Players/Player.hpp>
#include <Players/PlayersPool.hpp>
#include <Players/SpatialFramePlayers.hpp>
#include <AudioStorage.hpp>
#include <AudioRenderer.hpp>

#include <miniaudio.hpp>


class AudioEngine final {

public:

    AudioEngine ();

    AudioEngine (const AudioEngine&) = delete;
    AudioEngine& operator= (const AudioEngine&) = delete;
    AudioEngine (AudioEngine&&) = delete;
    AudioEngine& operator= (AudioEngine&&) = delete;

    ~AudioEngine ();

    static void callback (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    void start ();
    void stop ();

    PlayersPool&         getPlayersPool ();
    SpatialFramePlayers& getSpatialFramePlayers ();

private:

    void initDevice ();

private:

    ma_device device_{};

    AudioRenderer renderer_{};
};
