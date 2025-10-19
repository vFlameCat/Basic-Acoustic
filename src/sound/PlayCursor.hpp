#pragma once


#include "SoundStorage.hpp"


class PlayCursor final {

public:

    struct CreateInfo {

        double pos = 0.;
        double posOffset = 0.;

        float pitch = 1.f;
        float volume = 1.f;

        SoundStorage::SoundHandle soundHandle = SoundStorage::INVALID_SOUND_HANDLE;

        bool isLooped = true;
    };

    PlayCursor (CreateInfo info);

    float getSample () const;
    void advance ();

public:

    float pitch;
    float volume;

private:

    float getSampleInLoopedSound () const;
    float getSampleInUnloopedSound () const;

private:

    double pos_;
    double posOffset_;

    SoundStorage::SoundHandle soundHandle_;

    bool isLooped_;
};
