#pragma once


#include <AudioSourcesStorage.hpp>
#include <Vector3.hpp>

#include "raylib.h"


struct Listener {

    fc::Vector3f position = fc::Vector3f(0.f, 0.f, 0.f);
};


class SimulationManager final {

public:

    using SoundSourceHandle = uint64_t;
    static constexpr SoundSourceHandle INVALID_SOUND_SOURCE_HANDLE = 0;

    static constexpr float SoundSpeed = 343.f;

    SimulationManager () = default;

    SimulationManager (const SimulationManager&) = default;
    SimulationManager& operator= (const SimulationManager&) = default;

    template <typename CollisionFunc>
    void listenAroundCam (CollisionFunc collisionFunc) const;

public:

    Listener listener{};
    AudioSourcesStorage audioSources{};

private:

    template <typename CollisionFunc>
    void traceAudioSources (Ray ray, CollisionFunc collisionFunc, int depth = 10) const;

    double calcPosOffset (double distance) const;
    float  calcVolume (float distance) const;

    std::vector <Ray> genRaysAroundCam (int numRays) const;
};


#include "SimulationManager.inl"