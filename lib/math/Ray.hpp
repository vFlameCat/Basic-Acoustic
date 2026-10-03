#pragma once


#include <Vector3.hpp>


namespace rta {


struct Ray {

    Vector3f origin{};
    Vector3f direction{};
};


// Closest intersection of a ray with the scene.
struct RayHit {

    bool hit = false;

    float distance = 0.f;

    Vector3f point{};
    Vector3f normal{};
};


} // namespace rta
