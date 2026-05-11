#include "Player.hpp"

#include <cmath>


// (1) lerp between two closest samples to the pos_ + posOffset_
// (2) for performance purposes assumes that pos_ + posOffset_
//     won't change more than on audio_->size()
inline float Player::getSample () const {

    if (!isLooped_) {

        return getSampleInUnloopedSound();
    }

    return getSampleInLoopedSound();
}

inline void Player::advance () {

    pos_ += pitch;
}

inline float Player::getSampleInLoopedSound () const {

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

inline float Player::getSampleInUnloopedSound () const {

    Audio::size_type size = audio_->size();

    double advance = pos_ + posOffset_;
    if (advance < 0. || advance >= static_cast<double>(size)) {

        return 0.f;
    }

    uint64_t flooredAdvance = static_cast<uint64_t>(advance);     // correct due to advance >= 0.
    float t = static_cast<float>(advance - flooredAdvance);

    return std::lerp((*audio_)[flooredAdvance], (*audio_)[flooredAdvance + 1], t) * volume;
}
