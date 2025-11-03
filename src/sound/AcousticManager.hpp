#pragma once


#include <unordered_map>

#include "Scene.hpp"
#include "SoundPlayer.hpp"
#include "Vector3.hpp"


class AcousticManager final {

public:

    struct SoundSource {

        Vector3 position;
        SyncStaticPlayCursors::PlayCursorHandle playerHandle;
    };  

    using SoundSourceHandle = uint64_t;
    static constexpr SoundSourceHandle INVALID_SOUND_SOURCE_HANDLE = 0;

    static constexpr float SoundSpeed = 343.f;

    AcousticManager (const Scene *scene, const Camera *camera);

    AcousticManager (const AcousticManager&) = default;
    AcousticManager& operator= (const AcousticManager&) = default;

    SoundSourceHandle addSoundSource (SoundSource source);
    SoundSource& getSoundSource (SoundSourceHandle sourceHandle);
    void removeSoundSource (SoundSourceHandle sourceHandle);    

    //void checkSourcesVisibility () const;

    void listenAroundCam () const;

private:

    void traceSoundSources (Ray ray, int depth = 10) const;

    double calcPosOffset (double distance) const;
    float  calcVolume (float distance) const;

    std::vector <Ray> genRaysAroundCam (int numRays) const;

private:

    SoundSourceHandle nextHandle_ = 1;

    const Scene *scene_;
    const Camera *camera_;

    std::unordered_map <SoundSourceHandle, SoundSource> sources_{};
};