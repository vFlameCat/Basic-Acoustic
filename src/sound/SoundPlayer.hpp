#pragma once


#include <initializer_list>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "miniaudio.hpp"
#include "SoundStorage.hpp"
#include "PlayCursor.hpp"


class SoundPlayer final {

public:

    using PlayerHandle = int;
    static constexpr PlayerHandle INVALID_PLAYER_HANDLE = -1;

    SoundPlayer (const SoundPlayer&) = delete;
    SoundPlayer& operator= (const SoundPlayer&) = delete;

    ~SoundPlayer ();

    static void callbackPlayer (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    static SoundPlayer& getInstance ();

    void startPlayer ();
    void stopPlayer ();
    void switchPlayer ();

    PlayerHandle addStaticPlayer (PlayCursor player);
    PlayCursor& getStaticPlayer (PlayerHandle playerHandle);
    void removeStaticPlayer (PlayerHandle playerHandle);

    void addOneShotPlayer (PlayCursor player);
    void clearOneShotPlayers ();

private:

    SoundPlayer ();

    void initDevice ();

private:

    struct Players {

        std::unordered_map <SoundStorage::SoundHandle, PlayCursor> staticPlayers{};
        std::vector <PlayCursor> oneShotPlayers{};
    };

    PlayerHandle nextHandle_ = 1;

    ma_device device_{};
    bool isPlaying_ = false;

    Players players_{};
};