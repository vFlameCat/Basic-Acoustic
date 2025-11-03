#include "AcousticManager.hpp"
#include "Vector3.hpp"

#include <cmath>
#include <numbers>
#include <Vector3.hpp>


AcousticManager::AcousticManager (const Scene *scene, const Camera *camera):
  scene_(scene),
  camera_(camera) {

    if (!scene_ || !camera_) {

        std::cerr << "Error! Invalid Arguments in AcousticManager Ctor\n";
    }
}

AcousticManager::SoundSourceHandle AcousticManager::addSoundSource (SoundSource source) {

    sources_.emplace(nextHandle_, source);

    return nextHandle_++;
}

AcousticManager::SoundSource& AcousticManager::getSoundSource (SoundSourceHandle sourceHandle) {

    return sources_.at(sourceHandle);
}

void AcousticManager::removeSoundSource (SoundSourceHandle sourceHandle) {

    sources_.erase(sourceHandle);
}

// void AcousticManager::checkSourcesVisibility () const {

//     for (const auto& [key, source]: sources_) {

//         fc::Vector3f camPos(camera_->position), sourcePos(source.position);

//         float distanceToSource = fc::distance(camPos, sourcePos);

//         fc::Vector3f dirToSource = sourcePos - camPos;

//         RayCollision collision = scene_->getRayCollisionBoxes(Ray{camera_->position, dirToSource.normalize()});

//         PlayCursor& player = SoundPlayer::getInstance().staticPlayers.getPlayer(source.playerHandle);

//         if (collision.hit && collision.distance < distanceToSource) {

//             player.volume = 0.f;
//         }
//         else {

//             player.volume = 1.f;
//         }
//     }
// }

void AcousticManager::listenAroundCam () const {

    SoundPlayer& player = SoundPlayer::getInstance();

    player.staticPlayers.syncReadBuf();
    for (const auto& [key, source]: sources_) {

        fc::Vector3f rayPos(camera_->position), sourcePos(source.position);

        float distanceToSource = fc::distance(rayPos, sourcePos);

        fc::Vector3f dirToSource = sourcePos - rayPos;
        RayCollision collisionToSource = scene_->getRayCollisionBoxes(Ray{rayPos, dirToSource.normalize()});

        // if (!collisionToSource.hit || distanceToSource < collisionToSource.distance) {

        //     PlayCursor::CreateInfo info = player.staticPlayers.getPlayer(source.playerHandle).getInfo();
        //     info.posOffset = calcPosOffset(distanceToSource);
        //     info.volume = calcVolume(distanceToSource);

        //     player.oneShotPlayers.addPlayer(PlayCursor(info));
        // }

        PlayCursor::CreateInfo info = player.staticPlayers.getPlayer(source.playerHandle).getInfo();
        info.posOffset = calcPosOffset(distanceToSource);
        info.volume = calcVolume(distanceToSource);

        if (collisionToSource.hit && distanceToSource >= collisionToSource.distance) {

            info.volume *= 0.1f;
        }

        player.oneShotPlayers.addPlayer(PlayCursor(info));
    }

    std::vector <Ray> rays = genRaysAroundCam(32);      // no need to generate every frame
    for (auto ray: rays) {

        traceSoundSources(ray, 10);
    }

    player.oneShotPlayers.syncWriteBuf();
}

void AcousticManager::traceSoundSources (Ray ray, int depth) const {

    Ray curRay = ray;

    float curDistanceToListener = 0.f;
    float curVolumeDecr = 1.f;

    SoundPlayer& player = SoundPlayer::getInstance();

    for (int curDepth = 0; curDepth < depth; ++curDepth) {

        RayCollision collision = scene_->getRayCollisionBoxes(curRay);
        if (!collision.hit) {

            break;
        }

        fc::Vector3f incidentDir = curRay.direction;
        fc::Vector3f reflectDir = incidentDir - 2 * incidentDir.dot(collision.normal) * fc::Vector3f(collision.normal);

        curRay.position = fc::Vector3f(collision.point) + reflectDir * 0.1f;
        curRay.direction = reflectDir;

        curDistanceToListener += collision.distance;
        curVolumeDecr *= 0.6f;

        for (const auto& [key, source]: sources_) {

            fc::Vector3f rayPos(curRay.position), sourcePos(source.position);

            float distanceToSource = fc::distance(rayPos, sourcePos);

            fc::Vector3f dirToSource = sourcePos - rayPos;
            RayCollision collisionToSource = scene_->getRayCollisionBoxes(Ray{rayPos, dirToSource.normalize()});

            if (!collisionToSource.hit || distanceToSource < collisionToSource.distance) {

                PlayCursor::CreateInfo info = player.staticPlayers.getPlayer(source.playerHandle).getInfo();
                info.posOffset = calcPosOffset(curDistanceToListener + distanceToSource);
                info.volume = calcVolume(curDistanceToListener + distanceToSource);
                info.volume *= curVolumeDecr;

                player.oneShotPlayers.addPlayer(PlayCursor(info));
            }
        }
    }
}

double AcousticManager::calcPosOffset (double distance) const {

    return -distance / SoundSpeed * 48000;
}

float  AcousticManager::calcVolume (float distance) const {

    return 1.f / (distance * distance);
}

std::vector <Ray> AcousticManager::genRaysAroundCam (int numRays) const {

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