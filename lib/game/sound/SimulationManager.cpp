#include "SimulationManager.hpp"
#include "SyncPlayers.hpp"

#include <Vector3.hpp>
#include <cmath>
#include <numbers>
#include <AudioPlayer.hpp>
#include <iostream>


SimulationManager::SimulationManager (const Scene *scene, const Camera *camera):
  scene_(scene),
  camera_(camera) {

    if (!scene_ || !camera_) {

        std::cerr << "Error! Invalid Arguments in SimulationManager Ctor\n";
    }
}

void SimulationManager::listenAroundCam () const {

    AudioPlayer& player = AudioPlayer::getInstance();

    for (const auto &source: audioSources) {

        fc::Vector3f rayPos(camera_->position), sourcePos(source.position);

        float distanceToSource = fc::distance(rayPos, sourcePos);

        fc::Vector3f dirToSource = sourcePos - rayPos;
        RayCollision collisionToSource = scene_->getRayCollisionBoxes(Ray{rayPos, dirToSource.normalize()});

        SyncDynamicPlayCursors::DynamicPlayerCreateInfo info;
        info.playerHandle = source.handle;
        info.posOffset = calcPosOffset(distanceToSource);
        info.volume = calcVolume(distanceToSource);

        if (collisionToSource.hit && distanceToSource >= collisionToSource.distance) {

            info.volume *= 0.1f;
        }

        player.getDynamicPlayCursors().addPlayCursor(info);
    }

    std::vector <Ray> rays = genRaysAroundCam(32);      // no need to generate every frame
    for (auto ray: rays) {

        traceAudioSources(ray, 10);
    }

    player.getDynamicPlayCursors().dispatch();
}

void SimulationManager::traceAudioSources (Ray ray, int depth) const {

    Ray curRay = ray;

    float curVolumeDecr = 1.f;

    AudioPlayer& player = AudioPlayer::getInstance();

    for (int curDepth = 0; curDepth < depth; ++curDepth) {

        RayCollision collision = scene_->getRayCollisionBoxes(curRay);
        if (!collision.hit) {

            break;
        }

        fc::Vector3f incidentDir = curRay.direction;
        fc::Vector3f reflectDir = incidentDir - 2 * incidentDir.dot(collision.normal) * fc::Vector3f(collision.normal);

        curRay.position = fc::Vector3f(collision.point) + reflectDir * 0.1f;
        curRay.direction = reflectDir;

        curVolumeDecr *= 0.6f;

        for (const auto &source: audioSources) {

            fc::Vector3f rayPos(curRay.position), sourcePos(source.position);

            float distanceToSource = fc::distance(rayPos, sourcePos);

            fc::Vector3f dirToSource = sourcePos - rayPos;
            RayCollision collisionToSource = scene_->getRayCollisionBoxes(Ray{rayPos, dirToSource.normalize()});

            if (!collisionToSource.hit || distanceToSource < collisionToSource.distance) {

                SyncDynamicPlayCursors::DynamicPlayerCreateInfo info;
                info.playerHandle = source.handle;
                info.posOffset = calcPosOffset(distanceToSource);
                info.volume = calcVolume(distanceToSource);
                info.volume *= curVolumeDecr;

                player.getDynamicPlayCursors().addPlayCursor(info);
            }
        }
    }
}

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

        rays.emplace_back(Ray{camera_->position, dir});
    }

    return rays;
}