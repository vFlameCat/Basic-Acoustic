#pragma once


#include <AudioStorage.hpp>


class PlayCursor final {

public:

    struct CreateInfo {

        double pos = 0.;
        double posOffset = 0.;

        float pitch = 1.f;
        float volume = 1.f;

        AudioStorage::Handle audioHandle = AudioStorage::Handle::Invalid;

        bool isLooped = true;
    };

    PlayCursor (CreateInfo info);

    CreateInfo getInfo () const;

    float getSample () const;
    void advance ();

public:

    float pitch;
    float volume;

    double pos_;
    double posOffset_;

    AudioStorage::Handle audioHandle_;

private:

    float getSampleInLoopedSound () const;
    float getSampleInUnloopedSound () const;

private:

    bool isLooped_;
};
