#include "SampleManager.hpp"


SampleManager::SampleManager (std::string_view samplePath):
    pitch(1.f),
    volume(1.f),
    advance_(0.f),
    samples_(),
    decoder_() {

    initDecoder(samplePath);
    loadSamples();
};

SampleManager::~SampleManager () {

    ma_decoder_uninit(&decoder_);
}


// (1) lerp between two closest samples to the advance_
// (2) for performance purposes assuemes that advance_ won't change more thant on samples_.size()
float SampleManager::getSample () {

    if (advance_ < 0) {

        advance_ = static_cast<double>(samples_.size()) - advance_;
    }

    if (advance_ > static_cast<double>(samples_.size())) {

        advance_ = static_cast<double>(samples_.size()) - advance_;
    }

    std::size_t id = static_cast<std::size_t>(std::floor(advance_));

    float t = static_cast<float>(advance_ - std::floor(advance_));

    return std::lerp(samples_[id], samples_[id + 1], t) * volume;
}

void SampleManager::advance () {

    advance_ += pitch;
}


void SampleManager::initDecoder (std::string_view samplePath) {

    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 1, 48000);

    if (ma_decoder_init_file(samplePath.data(), &config, &decoder_) != MA_SUCCESS) {

        std::cerr << "An error occured during decoder initialization!\n";
        return;
    }
}

void SampleManager::loadSamples () {

    ma_uint64 frameCount = 0;
    if (ma_decoder_get_length_in_pcm_frames(&decoder_, &frameCount)) {

        std::cerr << "An error occured during sound loading!\n";
        return;
    }

    samples_.resize(frameCount);
    if (ma_decoder_read_pcm_frames(&decoder_, samples_.data(), frameCount, nullptr) != MA_SUCCESS) {

        std::cerr << "An error occured during sound loading!\n";
        return;
    }
}