#include "SyncPlayers.hpp"
#include <SimulationManager.hpp>
#include <AudioPlayer.hpp>


template <typename CollisionFunc>
void SimulationManager::listenAroundCam (CollisionFunc collisionFunc) const {

    SyncDynamicPlayCursors::FrameWriter players = player_->getDynamicPlayCursors().getFrameWriter();

    for (const auto &source: audioSources) {

        fc::Vector3f rayPos(listener.position), sourcePos(source.position);

        float distanceToSource = fc::distance(rayPos, sourcePos);

        fc::Vector3f dirToSource = sourcePos - rayPos;
        RayCollision collisionToSource = collisionFunc(Ray{rayPos, dirToSource.normalize()});

        SyncDynamicPlayCursors::DynamicPlayerCreateInfo info;
        info.playerHandle = source.handle;
        info.posOffset = calcPosOffset(distanceToSource);
        info.volume = calcVolume(distanceToSource);

        if (collisionToSource.hit && distanceToSource >= collisionToSource.distance) {

            info.volume *= 0.1f;
        }

        players.addPlayCursor(info);
    }

    std::vector <Ray> rays = genRaysAroundCam(32);      // TODO: could be optimized
    for (auto ray: rays) {

        traceAudioSources(players, ray, collisionFunc, 10);
    }
}

template <typename CollisionFunc>
void SimulationManager::traceAudioSources (SyncDynamicPlayCursors::FrameWriter &players, Ray ray, CollisionFunc collisionFunc, int depth) const {

    Ray curRay = ray;

    float curVolumeDecr = 1.f;
    float curPathLength = 0.f;

    for (int curDepth = 0; curDepth < depth; ++curDepth) {

        RayCollision collision = collisionFunc(curRay);
        if (!collision.hit) {

            break;
        }

        curPathLength += fc::distance(fc::Vector3f(curRay.position), fc::Vector3f(collision.point));

        fc::Vector3f incidentDir = curRay.direction;
        fc::Vector3f reflectDir = incidentDir - 2 * incidentDir.dot(collision.normal) * fc::Vector3f(collision.normal);

        curRay.position = fc::Vector3f(collision.point) + reflectDir * 0.1f;
        curRay.direction = reflectDir;

        curVolumeDecr *= 0.6f;

        for (const auto &source: audioSources) {

            fc::Vector3f rayPos(curRay.position), sourcePos(source.position);

            float distanceToSource = fc::distance(rayPos, sourcePos);

            fc::Vector3f dirToSource = sourcePos - rayPos;
            RayCollision collisionToSource = collisionFunc(Ray{rayPos, dirToSource.normalize()});

            if (!collisionToSource.hit || distanceToSource < collisionToSource.distance) {

                float totalDistance = curPathLength + distanceToSource;

                SyncDynamicPlayCursors::DynamicPlayerCreateInfo info;
                info.playerHandle = source.handle;
                info.posOffset = calcPosOffset(totalDistance);
                info.volume = calcVolume(totalDistance);
                info.volume *= curVolumeDecr;

                players.addPlayCursor(info);
            }
        }
    }
}