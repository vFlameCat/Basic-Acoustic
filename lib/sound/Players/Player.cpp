#include "Player.hpp"

#include <cmath>


Player::Player (CreateInfo info):
  pitch    (info.pitch),
  volume   (info.volume),
  pos_     (info.pos),
  posOffset_(info.posOffset),
  audio_   (&info.audio),
  isLooped_(info.isLooped) {
}


Player::CreateInfo Player::getInfo () const {

    return CreateInfo {
        .pos       = pos_,
        .posOffset = posOffset_,
        .pitch     = pitch,
        .volume    = volume,
        .audio     = *audio_,
        .isLooped  = isLooped_,
    };
}

// (1) lerp between two closest samples to the pos_ + posOffset_
// (2) for performance purposes assumes that pos_ + posOffset_
//     won't change more than on audio_->size()
float Player::getSample () const {

    if (!isLooped_) {

        return getSampleInUnloopedSound();
    }

    return getSampleInLoopedSound();
}

float Player::getSampleInLoopedSound () const {

    Audio::size_type size = audio_->size();

    double advance = pos_ + posOffset_;
    while (advance < 0) {

        advance += static_cast<double>(size);
    }

    while (advance >= static_cast<double>(size)) {

        advance -= static_cast<double>(size);
    }

    uint64_t flooredAdvance = static_cast<uint64_t>(advance);     // correct due to advance >= 0.
    float t = static_cast<float>(advance - flooredAdvance);

    return std::lerp((*audio_)[flooredAdvance], (*audio_)[flooredAdvance + 1], t) * volume;
}

float Player::getSampleInUnloopedSound () const {

    Audio::size_type size = audio_->size();

    double advance = pos_ + posOffset_;
    if (advance < 0. || advance >= static_cast<double>(size)) {

        return 0.f;
    }

    uint64_t flooredAdvance = static_cast<uint64_t>(advance);     // correct due to advance >= 0.
    float t = static_cast<float>(advance - flooredAdvance);

    return std::lerp((*audio_)[flooredAdvance], (*audio_)[flooredAdvance + 1], t) * volume;
}

void Player::advance () {

    pos_ += pitch;
}
