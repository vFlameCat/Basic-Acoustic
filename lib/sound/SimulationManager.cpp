#include "SimulationManager.hpp"

#include <Vector3.hpp>
#include <cmath>
#include <numbers>
#include <AudioPlayer.hpp>
#include <vector>


double SimulationManager::calcPosOffset (double distance) const {

    return -distance / SoundSpeed * 48000;
}

float  SimulationManager::calcVolume (float distance) const {

    return 1.f / (distance * distance);
}

std::vector <Ray> SimulationManager::genRaysAroundCam (int numRays) const {

    assert(numRays > 0);

    std::vector<Ray> rays;
    rays.reserve(numRays);

    float goldenRatio = (1.0f + std::sqrt(5.0f)) / 2.0f;    // may be could be constexpr since 26 standart

    for (int i = 0; i < numRays; ++i) {

        float theta = 2 * static_cast<float>(std::numbers::pi) * static_cast<float>(i) / goldenRatio;
        float phi = std::acos(1.0f - 2.0f * (static_cast<float>(i) + 0.5f) / static_cast<float>(numRays));
        
        fc::Vector3f dir (

            std::cos(theta) * std::sin(phi),
            std::sin(theta) * std::sin(phi),
            std::cos(phi)
        );

        rays.emplace_back(Ray{listener.position, dir});
    }

    return rays;
}