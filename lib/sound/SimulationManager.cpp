#include "SimulationManager.hpp"

#include <Vector3.hpp>
#include <algorithm>
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
  sphereDirections_(genSphereDirections(params.numRays)),
  perRayAmpWeight_(1.f / std::sqrt(static_cast<float>(params.numRays))) {}


double SimulationManager::calcPosOffset (double distance) const {

    return -distance / params_.soundSpeed * 48000;
}

float  SimulationManager::calcVolume (float distance) const {

    return 1.f / std::max(distance, params_.minDistToSource);
}

std::vector <fc::Vector3f> SimulationManager::genSphereDirections (uint32_t numRays) {

    assert(numRays > 0);

    std::vector<fc::Vector3f> directions;
    directions.reserve(numRays);

    float goldenRatio = (1.0f + std::sqrt(5.0f)) / 2.0f;

    for (uint32_t i = 0; i < numRays; ++i) {

        float theta = 2 * static_cast<float>(std::numbers::pi) * static_cast<float>(i) / goldenRatio;
        float phi = std::acos(1.0f - 2.0f * (static_cast<float>(i) + 0.5f) / static_cast<float>(numRays));
        
        fc::Vector3f dir (

            std::cos(theta) * std::sin(phi),
            std::sin(theta) * std::sin(phi),
            std::cos(phi)
        );

        directions.push_back(dir);
    }

    return directions;
}