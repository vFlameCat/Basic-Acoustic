#include <AudioPlayer.hpp>

#include <iostream>
#include <chrono>
#include <cassert>
#include <cmath>


AudioPlayer::AudioPlayer () {

    initDevice();  
}

AudioPlayer::~AudioPlayer () {

    ma_device_uninit(&device_);
}


void AudioPlayer::callbackPlayer (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {

    auto start = std::chrono::high_resolution_clock::now();

    AudioPlayer *player = static_cast<AudioPlayer*>(pDevice->pUserData);
    if (!player) {

        std::cerr << "Error! No data in callbackPlayer!\n";
        return;
    }

    player->audioRenderer.renderAudio(static_cast<float*>(pOutput), frameCount);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Callback duration: " << duration.count() << " microseconds\n";
}

void AudioPlayer::startPlayer () {

    ma_device_start(&device_);
}

void AudioPlayer::stopPlayer () {

    ma_device_stop(&device_);
}

SyncStaticPlayCursors& AudioPlayer::getStaticPlayCursors () {

    return audioRenderer.staticPlayCursors;
}

SyncDynamicPlayCursors& AudioPlayer::getDynamicPlayCursors () {

    return audioRenderer.dynamicPlayCursors;
}


void AudioPlayer::initDevice () {

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = 48000;
    config.dataCallback = callbackPlayer;
    config.pUserData = this;

    if (ma_device_init(NULL, &config, &device_) != MA_SUCCESS) {

        std::cerr << "An error occured during device initialization!\n";
        return;
    }
}