#include "PlayCursor.hpp"
#include <AudioStorage.hpp>

#include <iostream>
#include <cmath>


PlayCursor::PlayCursor (CreateInfo info):
  pitch       (info.pitch),
  volume      (info.volume),
  pos_        (info.pos),
  posOffset_  (info.posOffset),
  audioHandle_(info.audioHandle),
  isLooped_   (info.isLooped) {

    if (audioHandle_ == AudioStorage::Handle::Invalid) {

        std::cerr << "Error! Invalid sound handle!\n";
    }
}


PlayCursor::CreateInfo PlayCursor::getInfo () const {

    return CreateInfo {
        .pos         = pos_,
        .posOffset   = posOffset_,
        .pitch       = pitch,
        .volume      = volume,
        .audioHandle = audioHandle_,
        .isLooped    = isLooped_, 
    };
}

// (1) lerp between two closest samples to the pos_ + posOffset_
// (2) for performance purposes assuemes that pos_ + posOffset_ 
//     won't change more thant on SoundStorage::getInstance().getSound(soundHandle).size()
float PlayCursor::getSample () const {

    if (!isLooped_) {

        return getSampleInUnloopedSound();
    }

    return getSampleInLoopedSound();
}

float PlayCursor::getSampleInLoopedSound () const {

    const Audio &audio_ = AudioStorage::getInstance().getAudio(audioHandle_);       // TODO: fix !!!!!
    Audio::size_type size = audio_.size();

    double advance = pos_ + posOffset_;
    if (advance < 0) {

        advance += static_cast<double>(size);
    }

    if (advance >= static_cast<double>(size)) {

        advance -= static_cast<double>(size);
    }

    uint64_t flooredAdvance = static_cast<uint64_t>(advance);     // correct due to advance >= 0.
    float t = static_cast<float>(advance - flooredAdvance);

    return std::lerp(audio_[flooredAdvance], audio_[flooredAdvance + 1], t) * volume;
}

float PlayCursor::getSampleInUnloopedSound () const {

    const Audio &audio_ = AudioStorage::getInstance().getAudio(audioHandle_);       // TODO: fix !!!!!
    Audio::size_type size = audio_.size();

    double advance = pos_ + posOffset_;
    if (advance < 0. || advance >= static_cast<double>(size)) {

        return 0.f;
    }

    uint64_t flooredAdvance = static_cast<uint64_t>(advance);     // correct due to advance >= 0.
    float t = static_cast<float>(advance - std::floor(advance));

    return std::lerp(audio_[flooredAdvance], audio_[flooredAdvance + 1], t) * volume;
}

void PlayCursor::advance () {

    pos_ += pitch;
}