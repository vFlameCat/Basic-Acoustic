#pragma once


#include <Ray.hpp>
#include <Vector3.hpp>

#include <raylib.h>


// Conversions between raylib types and the audio library types.
// The library itself knows nothing about raylib — the demo converts at the boundary.

inline fc::Vector3f fromRl (::Vector3 vec) {

    return fc::Vector3f(vec.x, vec.y, vec.z);
}

inline ::Vector3 toRl (const fc::Vector3f &vec) {

    return ::Vector3{vec.x, vec.y, vec.z};
}

inline ::Ray toRl (const fc::Ray &ray) {

    return ::Ray{toRl(ray.origin), toRl(ray.direction)};
}

inline fc::RayHit fromRl (const ::RayCollision &collision) {

    return fc::RayHit{

        .hit      = collision.hit,
        .distance = collision.distance,
        .point    = fromRl(collision.point),
        .normal   = fromRl(collision.normal),
    };
}
