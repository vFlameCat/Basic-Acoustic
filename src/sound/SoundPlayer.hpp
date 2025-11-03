#pragma once


#include <initializer_list>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "miniaudio.hpp"
#include "SoundStorage.hpp"
#include "PlayCursor.hpp"
#include "SyncPlayers.hpp"


class SoundPlayer final {

public:

    using PlayerHandle = SyncStaticPlayers::PlayerHandle;

    SoundPlayer (const SoundPlayer&) = delete;
    SoundPlayer& operator= (const SoundPlayer&) = delete;

    ~SoundPlayer ();

    static void callbackPlayer (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    static SoundPlayer& getInstance ();

    void startPlayer ();
    void stopPlayer ();
    void switchPlayer ();

    SyncStaticPlayers staticPlayers{};
    SyncOneShotPlayers oneShotPlayers{};

private:

    SoundPlayer ();

    void initDevice ();

private:

    ma_device device_{};
    bool isDevicePlaying_ = false;
};