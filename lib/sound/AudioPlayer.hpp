#pragma once


#include <SyncPlayers.hpp>
#include <AudioStorage.hpp>
#include <PlayCursor.hpp>
#include <AudioRenderer.hpp>

#include <miniaudio.hpp>


class AudioPlayer final {

public:

    AudioPlayer (const AudioPlayer&) = delete;
    AudioPlayer& operator= (const AudioPlayer&) = delete;

    ~AudioPlayer ();

    static void callbackPlayer (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    static AudioPlayer& getInstance ();

    void startPlayer ();
    void stopPlayer ();
    void switchPlayer ();

    SyncStaticPlayCursors& getStaticPlayCursors ();
    SyncDynamicPlayCursors& getDynamicPlayCursors ();

private:

    AudioPlayer ();

    void initDevice ();

    struct CallbackData {

        AudioRenderer &audioRenderer;
    };

private:

    ma_device device_{};
    bool isDevicePlaying_ = false;

    AudioRenderer audioRenderer{};
};