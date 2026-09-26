#pragma once


#include <AudioSourcesStorage.hpp>
#include <Players/SpatialFramePlayers.hpp>
#include <Ray.hpp>
#include <Vector3.hpp>

#include <cstdint>
#include <type_traits>
#include <vector>


namespace rta {


class AudioEngine;


struct Listener {

    Vector3f position = Vector3f(0.f, 0.f, 0.f);
};


// Scene query the simulation relies on: returns the closest hit along the ray
// (the ray direction is normalized).
template <typename F>
concept RayCaster = std::is_invocable_r_v<RayHit, F&, const Ray&>;


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

    template <RayCaster CollisionFunc>
    void listenAroundCam (CollisionFunc collisionFunc) const;

public:

    Listener listener{};
    AudioSourcesStorage audioSources{};

private:

    template <RayCaster CollisionFunc>
    void traceAudioSources (SpatialFramePlayers::Writer &players, Ray ray, CollisionFunc collisionFunc, uint32_t depth) const;

    template <RayCaster CollisionFunc>
    void addContributionsAtPoint (SpatialFramePlayers::Writer &players,
                                  const Vector3f &point,
                                  float pathLength,
                                  float volume,
                                  float occlusionFactor,
                                  CollisionFunc collisionFunc) const;

    double calcPosOffset (double distance) const;
    float  calcVolume (float distance) const;

    static std::vector <Vector3f> genSphereDirections (uint32_t numRays);

private:

    AudioEngine &engine_;
    SimulationParams params_;

    std::vector <Vector3f> sphereDirections_;
    float perRayAmpWeight_;
};


} // namespace rta


#include "SimulationManager.inl"
