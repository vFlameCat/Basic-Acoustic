#include "SimulationManager.hpp"

#include <Vector3.hpp>
#include <cmath>
#include <numbers>
#include <AudioEngine.hpp>
#include <vector>
#include <cassert>


SimulationManager::SimulationManager (AudioEngine &engine):
  SimulationManager(engine, SimulationParams{}) {}

SimulationManager::SimulationManager (AudioEngine &engine, SimulationParams params):
  engine_(engine),
  params_(params),
  raysSphere_(genRaysSphere(params.numRays)),
  perRayAmpWeight_(1.f / std::sqrt(static_cast<float>(params.numRays))) {}


double SimulationManager::calcPosOffset (double distance) const {

    return -distance / params_.soundSpeed * 48000;
}

float  SimulationManager::calcVolume (float distance) const {

    return 1.f / std::max(distance, params_.minDistToSource);
}

std::vector <Ray> SimulationManager::genRaysSphere (uint32_t numRays) {

    assert(numRays > 0);

    std::vector<Ray> rays;
    rays.reserve(numRays);

    float goldenRatio = (1.0f + std::sqrt(5.0f)) / 2.0f;

    for (uint32_t i = 0; i < numRays; ++i) {

        float theta = 2 * static_cast<float>(std::numbers::pi) * static_cast<float>(i) / goldenRatio;
        float phi = std::acos(1.0f - 2.0f * (static_cast<float>(i) + 0.5f) / static_cast<float>(numRays));
        
        fc::Vector3f dir (

            std::cos(theta) * std::sin(phi),
            std::sin(theta) * std::sin(phi),
            std::cos(phi)
        );

        rays.emplace_back(Ray{fc::Vector3f(0.f, 0.f, 0.f), dir});
    }

    return rays;
}