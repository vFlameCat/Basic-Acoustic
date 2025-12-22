#pragma once


#include <Scene.hpp>
#include <AudioSourcesStorage.hpp>


class SimulationManager final {

public:

    using SoundSourceHandle = uint64_t;
    static constexpr SoundSourceHandle INVALID_SOUND_SOURCE_HANDLE = 0;

    static constexpr float SoundSpeed = 343.f;

    SimulationManager (const Scene *scene, const Camera *camera);

    SimulationManager (const SimulationManager&) = default;
    SimulationManager& operator= (const SimulationManager&) = default;

    void listenAroundCam () const;

public:

    AudioSourcesStorage audioSources{};

private:

    void traceAudioSources (Ray ray, int depth = 10) const;

    double calcPosOffset (double distance) const;
    float  calcVolume (float distance) const;

    std::vector <Ray> genRaysAroundCam (int numRays) const;

private:

    const Scene *scene_;
    const Camera *camera_;
};