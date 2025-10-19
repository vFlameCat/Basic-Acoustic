#pragma once


#include <unordered_map>

#include "Scene.hpp"
#include "SoundPlayer.hpp"


class AcousticManager final {

public:

    struct SoundSource {

        Vector3 position;
        SoundPlayer::PlayerHandle playerHandle;
    };  

    using SoundSourceHandle = int;
    static constexpr SoundSourceHandle INVALID_SOUND_SOURCE_HANDLE = -1;

    AcousticManager (const Scene *scene, const Camera *camera);

    AcousticManager (const AcousticManager&) = default;
    AcousticManager& operator= (const AcousticManager&) = default;

    SoundSourceHandle addSoundSource (SoundSource source);
    SoundSource& getSoundSource (SoundSourceHandle sourceHandle);
    void removeSoundSource (SoundSourceHandle sourceHandle);    

    void checkSourcesVisibility () const;

    void listenAroundCam () const;

private:

    void traceSoundSources () const;

    std::vector <Ray> genRaysAroundCam (int numRays) const;

private:

    SoundSourceHandle nextHandle_ = 1;

    const Scene *scene_;
    const Camera *camera_;

    std::unordered_map <SoundSourceHandle, SoundSource> sources_{};
};