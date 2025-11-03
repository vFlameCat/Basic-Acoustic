#include "SoundStorage.hpp"

#include <iostream>
#include <cassert>


SoundStorage::Sound::Sound (std::vector <float> &&samples):
    samples_(std::move(samples)) {}


float SoundStorage::Sound::operator[] (std::size_t pos) const {

    return samples_[pos];
}

float SoundStorage::Sound::at (std::size_t pos) const {

    return samples_.at(pos);
}

std::size_t SoundStorage::Sound::size () const {

    return samples_.size();
}


SoundStorage& SoundStorage::getInstance () {

    static SoundStorage instance;
    return instance;
}

SoundStorage::SoundHandle SoundStorage::loadSound (const std::string &samplePath) {

    std::vector <float> samples;
    if (decodeAndLoadFloatSamples(samplePath, samples)) {

        return INVALID_SOUND_HANDLE;
    }

    sounds_.emplace(nextHandle_, Sound(std::move(samples)));

    return nextHandle_++;
}

void SoundStorage::unloadSound (SoundHandle soundHandle) {

    sounds_.erase(soundHandle);
}

const SoundStorage::Sound& SoundStorage::getSound (SoundHandle soundHandle) const {

    return sounds_.at(soundHandle);
}

int SoundStorage::decodeAndLoadFloatSamples (const std::string &samplePath, std::vector <float> &samples) const {

    ma_decoder decoder = {};
    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 1, 48000);
    if (ma_decoder_init_file(samplePath.c_str(), &config, &decoder) != MA_SUCCESS) {

        std::cerr << "An error occured during loading sound: " + samplePath + " !\n";
        return -1;
    }

    struct DecoderGuard {

        ma_decoder* decoder;
        ~DecoderGuard() { ma_decoder_uninit(decoder); }

    } guard{&decoder};

    ma_uint64 frameCount = 0;
    if (ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount) != MA_SUCCESS) {

        std::cerr << "An error occured during loading sound: " + samplePath + " !\n";
        return -1;
    }

    samples.resize(frameCount);
    if (ma_decoder_read_pcm_frames(&decoder, samples.data(), frameCount, nullptr) != MA_SUCCESS) {

        std::cerr << "An error occured during loading sound: " + samplePath + " !\n";
        return -1;
    }    

    return 0;
}