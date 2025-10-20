#include "PlayCursor.hpp"

#include <iostream>
#include <cmath>


PlayCursor::PlayCursor (CreateInfo info):
  pitch       (info.pitch),
  volume      (info.volume),
  pos_        (info.pos),
  posOffset_  (info.posOffset),
  soundHandle_(info.soundHandle),
  isLooped_   (info.isLooped) {

    if (soundHandle_ == SoundStorage::INVALID_SOUND_HANDLE) {

        std::cerr << "Error! Invalid sound handle!\n";
    }
}


PlayCursor::CreateInfo PlayCursor::getInfo () const {

    return CreateInfo{
        .pos         = pos_,
        .posOffset   = posOffset_,
        .pitch       = pitch,
        .volume      = volume,
        .soundHandle = soundHandle_,
        .isLooped    = isLooped_, 
    };
}

// (1) lerp between two closest samples to the pos_ + posOffset_
// (2) for performance purposes assuemes that pos_ + posOffset_ 
//     won't change more thant on SoundStorage::getInstance().getSound(soundHandle).size()
float PlayCursor::getSample () const {

    if (SoundStorage::getInstance().getSound(soundHandle_).size() == 0) {

        std::cerr << "Error! Invalid sound!\n";
        return 0.f;
    }

    if (!isLooped_) {

        return getSampleInUnloopedSound();
    }

    return getSampleInLoopedSound();
}

float PlayCursor::getSampleInLoopedSound () const {

    const SoundStorage::Sound &samples = SoundStorage::getInstance().getSound(soundHandle_);
    std::size_t size = samples.size();

    double advance = pos_ + posOffset_;

    if (advance < 0) {

        advance += static_cast<double>(size);
    }

    if (advance >= static_cast<double>(size)) {

        advance -= static_cast<double>(size);
    }

    std::size_t id = static_cast<std::size_t>(std::floor(advance));
    std::size_t nextId = (id + 1) % size;

    float t = static_cast<float>(advance - std::floor(advance));

    return std::lerp(samples[id], samples[nextId], t) * volume;
}

float PlayCursor::getSampleInUnloopedSound () const {

    const SoundStorage::Sound &samples = SoundStorage::getInstance().getSound(soundHandle_);
    std::size_t size = samples.size();

    double advance = pos_ + posOffset_;

    if (advance < 0. || advance > static_cast<double>(size)) {

        return 0.f;
    }

    std::size_t id = static_cast<std::size_t>(std::floor(advance));
    std::size_t nextId = id + 1;
    if (nextId >= size) {

        return samples[id] * volume;
    }

    float t = static_cast<float>(advance - std::floor(advance));

    return std::lerp(samples[id], samples[nextId], t) * volume;
}

void PlayCursor::advance () {

    pos_ += pitch;
}