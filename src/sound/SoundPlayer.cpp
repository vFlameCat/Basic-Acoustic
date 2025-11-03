#include "SoundPlayer.hpp"

#include <iostream>
#include <chrono>
#include <cassert>
#include <cmath>


namespace {

float applySoftLimiting (float sample) {

    if (sample > 1.0f) {

        return 1.0f - (1.0f / (sample + 1.0f));
    } 
    else if (sample < -1.0f) {

        return -1.0f + (1.0f / (-sample + 1.0f));
    }
    return sample;
}


float applyLimiting (float sample) {

    static float threshold_ = 0.9f;
    static float release_ = 0.999f; 
    static float envelope_ = 0.0f;

    float absSample = std::abs(sample);
    if (absSample > threshold_) {

        envelope_ = std::max(envelope_ * release_, absSample);
        float gainReduction = threshold_ / envelope_;
        sample *= gainReduction;
    } 
    else {

        envelope_ *= release_;
    }
    
    return applySoftLimiting(sample);
}


struct CallbackData {

    SyncStaticPlayers &staticPlayers;
    SyncOneShotPlayers &oneShotPlayers;
};

    
}



std::mutex oneShotPlayerSync;


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

    SyncStaticPlayers &staticPlayers = data->staticPlayers;
    SyncOneShotPlayers &oneShotPlayers = data->oneShotPlayers;


    oneShotPlayers.syncReadBuf();


    for (auto& player: oneShotPlayers.readBuf_) {

        player.pos_ = staticPlayers.modifyBuf_.at(1).pos_;
    }

    for (ma_uint32 i = 0; i < frameCount; ++i) {

        *(static_cast<float*>(pOutput) + i) = 0;

        for (auto& player: oneShotPlayers.readBuf_) {

            *(static_cast<float*>(pOutput) + i) += player.getSample();
            player.advance();
        }

        for (auto& [handle, player]: staticPlayers.modifyBuf_) {

            *(static_cast<float*>(pOutput) + i) += player.getSample();
            player.advance();
        } 
    }


    staticPlayers.syncModifyBuf();


    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // std::cout << "Players number: " << oneShotPlayers.readBuf_.size() << std::endl;
    // std::cout << "Callback duration: " << duration.count() << " microseconds\n";
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


void SoundPlayer::initDevice () {

    static CallbackData data {

        .staticPlayers = staticPlayers,
        .oneShotPlayers = oneShotPlayers,
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