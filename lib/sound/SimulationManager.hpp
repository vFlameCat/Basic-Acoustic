#pragma once


#include <AudioSourcesStorage.hpp>
#include <Players/SpatialFramePlayers.hpp>
#include <Vector3.hpp>

#include "raylib.h"


class AudioEngine;


struct Listener {

    fc::Vector3f position = fc::Vector3f(0.f, 0.f, 0.f);
};


class SimulationManager final {

public:

    static constexpr float SoundSpeed = 343.f;

public:

    explicit SimulationManager (AudioEngine &engine);

    SimulationManager (const SimulationManager&) = delete;
    SimulationManager& operator= (const SimulationManager&) = delete;

    template <typename CollisionFunc>
    void listenAroundCam (CollisionFunc collisionFunc) const;

public:

    Listener listener{};
    AudioSourcesStorage audioSources{};

private:

    template <typename CollisionFunc>
    void traceAudioSources (SpatialFramePlayers::Writer &players, Ray ray, CollisionFunc collisionFunc, int depth = 10) const;

    double calcPosOffset (double distance) const;
    float  calcVolume (float distance) const;

    std::vector <Ray> genRaysAroundCam (int numRays) const;

private:

    AudioEngine *engine_;
};


#include "SimulationManager.inl"