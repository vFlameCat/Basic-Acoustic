#pragma once


#include <AudioSourcesStorage.hpp>
#include <Players/SpatialFramePlayers.hpp>
#include <Vector3.hpp>
#include <cstdint>

#include "raylib.h"


class AudioEngine;


struct Listener {

    fc::Vector3f position = fc::Vector3f(0.f, 0.f, 0.f);
};


class SimulationManager final {

public:

    struct SimulationParams {

        double soundSpeed = 343.;

        float minDistToSource = 0.5f;
        float minVolume = 0.01f;

        float reflectionAmp = 0.8f;
        float occlusionAmp  = 0.1f;

        uint32_t numRays = 32;
        uint32_t depth = 10;
    };

public:

    explicit SimulationManager (AudioEngine &engine);
    SimulationManager (AudioEngine &engine, SimulationParams params);

    SimulationManager (const SimulationManager&) = delete;
    SimulationManager& operator= (const SimulationManager&) = delete;

    template <typename CollisionFunc>
    void listenAroundCam (CollisionFunc collisionFunc) const;

public:

    Listener listener{};
    AudioSourcesStorage audioSources{};

private:

    template <typename CollisionFunc>
    void traceAudioSources (SpatialFramePlayers::Writer &players, Ray ray, CollisionFunc collisionFunc, uint32_t depth) const;

    template <typename CollisionFunc>
    void addContributionsAtPoint (SpatialFramePlayers::Writer &players,
                                  const fc::Vector3f &point,
                                  float pathLength,
                                  float volume,
                                  float occlusionFactor,
                                  CollisionFunc collisionFunc) const;

    double calcPosOffset (double distance) const;
    float  calcVolume (float distance) const;

    static std::vector <Ray> genRaysSphere (uint32_t numRays);

private:

    AudioEngine &engine_;
    SimulationParams params_;

    std::vector <Ray> raysSphere_;
    float perRayAmpWeight_;
};


#include "SimulationManager.inl"