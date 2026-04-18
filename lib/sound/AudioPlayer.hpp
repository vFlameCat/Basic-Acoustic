#pragma once


#include <SyncPlayers.hpp>
#include <AudioStorage.hpp>
#include <PlayCursor.hpp>
#include <AudioRenderer.hpp>

#include <miniaudio.hpp>


class AudioPlayer final {

public:

    AudioPlayer ();

    AudioPlayer (const AudioPlayer&) = delete;
    AudioPlayer& operator= (const AudioPlayer&) = delete;
    AudioPlayer (AudioPlayer&&) = delete;
    AudioPlayer& operator= (AudioPlayer&&) = delete;

    ~AudioPlayer ();

    static void callbackPlayer (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    void startPlayer ();
    void stopPlayer ();

    SyncStaticPlayCursors& getStaticPlayCursors ();
    SyncDynamicPlayCursors& getDynamicPlayCursors ();

private:

    void initDevice ();

private:

    ma_device device_{};

    AudioRenderer audioRenderer{};
};