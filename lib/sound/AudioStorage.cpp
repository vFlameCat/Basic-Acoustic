#include <AudioStorage.hpp>

#include <iostream>
#include <utility>
#include <vector>


Audio::Audio (std::vector <float> &&samples):
    samples_(std::move(samples)) {}


float Audio::operator[] (std::size_t pos) const {

    return samples_[pos];
}

float Audio::at (std::size_t pos) const {

    return samples_.at(pos);
}

Audio::size_type Audio::size () const {

    return samples_.size() - 1;
}


AudioStorage& AudioStorage::getInstance () {

    static AudioStorage instance;
    return instance;
}

AudioStorage::Handle AudioStorage::loadAudio (const std::string &samplePath) {

    std::vector <float> samples;
    if (decodeAndLoadFloatSamples(samplePath, samples)) {

        return Handle::Invalid;
    }

    Handle handle = static_cast<Handle>(storage_.size());
    storage_.emplace_back(std::make_unique<Audio>(std::move(samples)));

    return handle;
}

void AudioStorage::unloadAudio (Handle handle) {

    storage_[static_cast<std::vector<std::unique_ptr<Audio>>::size_type>(handle)].reset();
}

const Audio& AudioStorage::getAudio (Handle handle) const {

    return *storage_[static_cast<std::vector<std::unique_ptr<Audio>>::size_type>(handle)];
}

int AudioStorage::decodeAndLoadFloatSamples (const std::string &samplePath, std::vector <float> &samples) const {

    ma_decoder decoder = {};
    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 1, 48000);
    if (ma_decoder_init_file(samplePath.c_str(), &config, &decoder) != MA_SUCCESS) {

        std::cerr << "An error occured during loading audio: " + samplePath + " !\n";
        return -1;
    }

    struct DecoderGuard {

        ma_decoder* decoder;
        ~DecoderGuard() { ma_decoder_uninit(decoder); }

    } guard{&decoder};

    ma_uint64 frameCount = 0;
    if (ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount) != MA_SUCCESS) {

        std::cerr << "An error occured during loading audio: " + samplePath + " !\n";
        return -1;
    }

    samples.resize(frameCount + 1);
    if (ma_decoder_read_pcm_frames(&decoder, samples.data(), frameCount, nullptr) != MA_SUCCESS) {

        std::cerr << "An error occured during loading audio: " + samplePath + " !\n";
        return -1;
    }    
    samples[frameCount] = samples[0];   // so we can process looped audios more effectively

    return 0;
}