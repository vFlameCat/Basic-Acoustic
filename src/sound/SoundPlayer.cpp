#include "SoundPlayer.hpp"

#include <iostream>


SoundPlayer::SoundPlayer () {

    initDevice();
    players_.oneShotPlayers.reserve(64);    // maybe change 
}

SoundPlayer::~SoundPlayer () {

    ma_device_uninit(&device_);
}


void SoundPlayer::callbackPlayer (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {

    Players *players = static_cast<Players*>(pDevice->pUserData);
    if (!players) {

        std::cerr << "Error! No data in callbackPlayer!\n";
        return;
    }

    for (ma_uint32 i = 0; i < frameCount; ++i) {

        *(static_cast<float*>(pOutput) + i) = 0;

        for (auto& [handle, player]: players->staticPlayers) {

            *(static_cast<float*>(pOutput) + i) += player.getSample();
            player.advance();
        } 

        for (auto& player: players->oneShotPlayers) {

            *(static_cast<float*>(pOutput) + i) += player.getSample();
            player.advance();
        } 
    }
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

    if(isPlaying_) {

        stopPlayer();
    }
    else {

        startPlayer();
    }
}

SoundPlayer::PlayerHandle SoundPlayer::addStaticPlayer (PlayCursor player) {

    players_.staticPlayers.emplace(nextHandle_, player);

    return nextHandle_++;
}

PlayCursor& SoundPlayer::getStaticPlayer (PlayerHandle playerHandle) {

    return players_.staticPlayers.at(playerHandle);
}

void SoundPlayer::removeStaticPlayer (PlayerHandle playerHandle) {

    players_.staticPlayers.erase(playerHandle);
}

void SoundPlayer::addOneShotPlayer (PlayCursor player) {

    players_.oneShotPlayers.push_back(player);
}

void SoundPlayer::clearOneShotPlayers () {

    players_.oneShotPlayers.clear();
}

void SoundPlayer::initDevice () {

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = 48000;
    config.dataCallback = callbackPlayer;
    config.pUserData = &players_;

    if (ma_device_init(NULL, &config, &device_) != MA_SUCCESS) {
        
        std::cerr << "An error occured during device initialization!\n";
        return;
    }
}