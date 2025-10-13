#include "SceneSoundPlayer.hpp"


SceneSoundPlayer::SceneSoundPlayer ():
  sampleManagers_(),
  device_() {

    initDevice();
    ma_device_start(&device_);
}

SceneSoundPlayer::~SceneSoundPlayer () {

    ma_device_uninit(&device_);
}


void SceneSoundPlayer::addSampleManager (SampleManager *manager) {

    if (!manager) {

        std::cerr << "Errror! manager is nullptr in addSampleManager";
        return;
    }

    sampleManagers_.push_back(manager);
}

void SceneSoundPlayer::callbakcPlayer (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {

    std::vector <SampleManager*> *managers = static_cast<std::vector <SampleManager*>*>(pDevice->pUserData);
    if (!managers) {

        std::cerr << "Error! No data in callbackPlayer!\n";
        return;
    }

    for (ma_uint32 i = 0; i < frameCount; ++i) {        //may be chande nested loops order

        *(static_cast<float*>(pOutput) + i) = 0;
        for (SampleManager *manager: *managers) {

            *(static_cast<float*>(pOutput) + i) += manager->getSample();
            manager->advance();
        } 
    }
}


void SceneSoundPlayer::initDevice () {

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = 48000;
    config.dataCallback = callbakcPlayer;
    config.pUserData = &sampleManagers_;

    if (ma_device_init(NULL, &config, &device_) != MA_SUCCESS) {
        
        std::cerr << "An error occured during device initialization!\n";
        return;
    }
}