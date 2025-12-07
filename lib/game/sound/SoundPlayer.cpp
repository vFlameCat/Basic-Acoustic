#include "SoundPlayer.hpp"

#include <iostream>
#include <chrono>
#include <cassert>
#include <cmath>


SoundPlayer::SoundPlayer () {

    initDevice();  
}

SoundPlayer::~SoundPlayer () {

    ma_device_uninit(&device_);
}


void SoundPlayer::callbackPlayer (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {

    auto start = std::chrono::high_resolution_clock::now();

    CallbackData *data = static_cast<CallbackData*>(pDevice->pUserData);
    if (!data) {

        std::cerr << "Error! No data in callbackPlayer!\n";
        return;
    }

    data->audioRenderer.renderAudio(static_cast<float*>(pOutput), frameCount);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Callback duration: " << duration.count() << " microseconds\n";
}

SoundPlayer& SoundPlayer::getInstance () {

    static SoundPlayer instance;
    return instance;
}

void SoundPlayer::startPlayer () {

    ma_device_start(&device_);
}

void SoundPlayer::stopPlayer () {

    ma_device_stop(&device_);
}

void SoundPlayer::switchPlayer () {

    if(isDevicePlaying_) {

        stopPlayer();
    }
    else {

        startPlayer();
    }
}


SoundPlayer::PlayCursorHandle SoundPlayer::addStaticPlayCursor (PlayCursor staticPlayCursor) {

    return audioRenderer.staticPlayCursors.addPlayCursor(staticPlayCursor);
}

PlayCursor& SoundPlayer::getStaticPlayCursor (PlayCursorHandle handle) & {

    return audioRenderer.staticPlayCursors.getPlayCursor(handle);
}

void SoundPlayer::removeStaticPlayCursor (PlayCursorHandle handle) {

    audioRenderer.staticPlayCursors.removePlayCursor(handle);
}

void SoundPlayer::addDynamicPlayCursor (PlayCursor dynamicPlayCursor) {

    audioRenderer.dynamicPlayCursors.addPlayCursor(dynamicPlayCursor);
}

void SoundPlayer::addDynamicPlayCursor (DynamicPlayerCreateInfo info) {

    audioRenderer.dynamicPlayCursors.addPlayCursor(info);
}

void SoundPlayer::dispatchDynamicPlayCursors () {

    audioRenderer.dynamicPlayCursors.dispatch();
}


void SoundPlayer::initDevice () {

    static CallbackData data {

        .audioRenderer = audioRenderer,
    };

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = 48000;
    config.dataCallback = callbackPlayer;
    config.pUserData = &data;

    if (ma_device_init(NULL, &config, &device_) != MA_SUCCESS) {
        
        std::cerr << "An error occured during device initialization!\n";
        return;
    }
}