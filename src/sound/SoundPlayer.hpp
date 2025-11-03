#pragma once


#include <initializer_list>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "miniaudio.hpp"
#include "SoundStorage.hpp"
#include "PlayCursor.hpp"
#include <AudioRenderer.hpp>


class SoundPlayer final {

public:

    using PlayCursorHandle = SyncStaticPlayCursors::PlayCursorHandle;
    static constexpr PlayCursorHandle INVALID_PLAYER_HANDLE = SyncStaticPlayCursors::INVALID_PLAYER_HANDLE;

    using DynamicPlayerCreateInfo = SyncDynamicPlayCursors::DynamicPlayerCreateInfo;

public:

    SoundPlayer (const SoundPlayer&) = delete;
    SoundPlayer& operator= (const SoundPlayer&) = delete;

    ~SoundPlayer ();

    static void callbackPlayer (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    static SoundPlayer& getInstance ();

    void startPlayer ();
    void stopPlayer ();
    void switchPlayer ();

    PlayCursorHandle addStaticPlayCursor (PlayCursor staticPlayCursor);
    PlayCursor& getStaticPlayCursor (PlayCursorHandle handle) &;
    void removeStaticPlayCursor (PlayCursorHandle handle);

    void addDynamicPlayCursor (PlayCursor dynamicPlayCursor);
    void addDynamicPlayCursor (DynamicPlayerCreateInfo info);
    void dispatchDynamicPlayCursors ();

private:

    SoundPlayer ();

    void initDevice ();

    struct CallbackData {

        AudioRenderer &audioRenderer;
    };

private:

    ma_device device_{};
    bool isDevicePlaying_ = false;

    AudioRenderer audioRenderer{};
};