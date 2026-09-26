#include <Players/SpatialFramePlayers.hpp>
#include <SimulationManager.hpp>
#include <AudioEngine.hpp>
#include <Ray.hpp>


namespace rta {


template <RayCaster CollisionFunc>
void SimulationManager::listenAroundCam (CollisionFunc collisionFunc) const {

    SpatialFramePlayers::Writer players = engine_.getSpatialFramePlayers().getWriter();

    addContributionsAtPoint(players, listener.position, 0.f, 1.f, params_.occlusionAmp, collisionFunc);

    for (const auto &direction: sphereDirections_) {

        traceAudioSources(players, Ray{listener.position, direction}, collisionFunc, params_.depth);
    }
}

template <RayCaster CollisionFunc>
void SimulationManager::traceAudioSources (SpatialFramePlayers::Writer &players, Ray ray, CollisionFunc collisionFunc, uint32_t depth) const {

    Ray curRay = ray;

    float curVolume = 1.f;
    float curPathLength = 0.f;

    for (uint32_t curDepth = 0; curDepth < depth; ++curDepth) {

        if (curVolume < params_.minVolume) break;

        RayHit collision = collisionFunc(curRay);
        if (!collision.hit) {

            break;
        }

        curPathLength += distance(curRay.origin, collision.point);

        Vector3f incidentDir = curRay.direction;
        Vector3f reflectDir = incidentDir - 2 * incidentDir.dot(collision.normal) * collision.normal;

        curRay.origin = collision.point + reflectDir * 0.1f;
        curRay.direction = reflectDir;

        curVolume *= params_.reflectionAmp;

        addContributionsAtPoint(players, curRay.origin, curPathLength, curVolume * perRayAmpWeight_, 0.f, collisionFunc);
    }
}

template <RayCaster CollisionFunc>
void SimulationManager::addContributionsAtPoint (SpatialFramePlayers::Writer &players,
                                                 const Vector3f &point,
                                                 float pathLength,
                                                 float volume,
                                                 float occlusionFactor,
                                                 CollisionFunc collisionFunc) const {

    for (const auto &source: audioSources) {

        Vector3f sourcePos(source.position);

        float distanceToSource = distance(point, sourcePos);

        Vector3f dirToSource = sourcePos - point;
        RayHit collisionToSource = collisionFunc(Ray{point, dirToSource.normalize()});

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


} // namespace rta
