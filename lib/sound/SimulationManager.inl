#include "raylib.h"
#include <Players/SpatialFramePlayers.hpp>
#include <SimulationManager.hpp>
#include <AudioEngine.hpp>


template <typename CollisionFunc>
void SimulationManager::listenAroundCam (CollisionFunc collisionFunc) const {

    SpatialFramePlayers::Writer players = engine_.getSpatialFramePlayers().getWriter();

    addContributionsAtPoint(players, listener.position, 0.f, 1.f, params_.occlusionAmp, collisionFunc);

    for (auto &ray: raysSphere_) {

        Ray dir = Ray(Vector3(listener.position.x + ray.position.x, listener.position.y + ray.position.y, listener.position.z + ray.position.z), ray.direction);
        traceAudioSources(players, dir, collisionFunc, params_.depth);
    }
}

template <typename CollisionFunc>
void SimulationManager::traceAudioSources (SpatialFramePlayers::Writer &players, Ray ray, CollisionFunc collisionFunc, uint32_t depth) const {

    Ray curRay = ray;

    float curVolume = 1.f;
    float curPathLength = 0.f;

    for (uint32_t curDepth = 0; curDepth < depth; ++curDepth) {

        if (curVolume < params_.minVolume) break;

        RayCollision collision = collisionFunc(curRay);
        if (!collision.hit) {

            break;
        }

        curPathLength += fc::distance(fc::Vector3f(curRay.position), fc::Vector3f(collision.point));

        fc::Vector3f incidentDir = curRay.direction;
        fc::Vector3f reflectDir = incidentDir - 2 * incidentDir.dot(collision.normal) * fc::Vector3f(collision.normal);

        curRay.position = fc::Vector3f(collision.point) + reflectDir * 0.1f;
        curRay.direction = reflectDir;

        curVolume *= params_.reflectionAmp;

        addContributionsAtPoint(players, fc::Vector3f(curRay.position), curPathLength, curVolume * perRayAmpWeight_, 0.f, collisionFunc);
    }
}

template <typename CollisionFunc>
void SimulationManager::addContributionsAtPoint (SpatialFramePlayers::Writer &players,
                                                 const fc::Vector3f &point,
                                                 float pathLength,
                                                 float volume,
                                                 float occlusionFactor,
                                                 CollisionFunc collisionFunc) const {

    for (const auto &source: audioSources) {

        fc::Vector3f sourcePos(source.position);

        float distanceToSource = fc::distance(point, sourcePos);

        fc::Vector3f dirToSource = sourcePos - point;
        RayCollision collisionToSource = collisionFunc(Ray{point, dirToSource.normalize()});

        bool occluded = collisionToSource.hit && distanceToSource >= collisionToSource.distance;

        if (occluded && occlusionFactor == 0.f) continue;

        float totalDistance = pathLength + distanceToSource;

        SpatialFramePlayers::PlayerCreateInfo info;
        info.playerHandle = source.handle;
        info.posOffset = calcPosOffset(totalDistance);
        info.volume = calcVolume(totalDistance) * volume;

        if (occluded) info.volume *= occlusionFactor;

        players.addPlayer(info);
    }
}
