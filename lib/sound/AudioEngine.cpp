#include <AudioEngine.hpp>

#include <iostream>
#include <chrono>
#include <cassert>
#include <cmath>


AudioEngine::AudioEngine () {

    initDevice();
}

AudioEngine::~AudioEngine () {

    ma_device_uninit(&device_);
}


void AudioEngine::callback (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {

    auto start = std::chrono::high_resolution_clock::now();

    AudioEngine *engine = static_cast<AudioEngine*>(pDevice->pUserData);
    if (!engine) {

        std::cerr << "Error! No data in AudioEngine callback!\n";
        return;
    }

    engine->renderer_.renderAudio(static_cast<float*>(pOutput), frameCount);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Callback duration: " << duration.count() << " microseconds\n";
}

void AudioEngine::start () {

    ma_device_start(&device_);
}

void AudioEngine::stop () {

    ma_device_stop(&device_);
}

PlayersPool& AudioEngine::getPlayersPool () {

    return renderer_.playersPool;
}

SpatialFramePlayers& AudioEngine::getSpatialFramePlayers () {

    return renderer_.spatialFramePlayers;
}


void AudioEngine::initDevice () {

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = 48000;
    config.dataCallback = callback;
    config.pUserData = this;

    if (ma_device_init(NULL, &config, &device_) != MA_SUCCESS) {

        std::cerr << "An error occured during device initialization!\n";
        return;
    }
}
