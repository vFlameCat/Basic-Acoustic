#include "AcousticManager.hpp"
#include "Vector3.hpp"

#include <cmath>
#include <numbers>


float getDistance (Vector3 pointA, Vector3 pointB) {

    float diffX = pointB.x - pointA.x;
    float diffY = pointB.y - pointA.y;
    float diffZ = pointB.z - pointA.z;

    return std::sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);
}

Vector3 normalize (Vector3 vec) {

    float len = getDistance(Vector3{0, 0, 0}, vec);

    return Vector3{vec.x / len, vec.y / len, vec.z / len};
}


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

void AcousticManager::checkSourcesVisibility () const {

    for (const auto& [key, source]: sources_) {

        float distanceToSource = getDistance(camera_->position, source.position);

        Vector3 rayDir = Vector3{source.position.x - camera_->position.x,
                                 source.position.y - camera_->position.y,
                                 source.position.z - camera_->position.z};

        RayCollision collision = scene_->getRayCollisionBoxes(Ray{camera_->position, normalize(rayDir)});

        PlayCursor& player = SoundPlayer::getInstance().getStaticPlayer(source.playerHandle);

        if (collision.hit && collision.distance < distanceToSource) {

            player.volume = 0.f;
        }
        else {

            player.volume = 1.f;
        }
    }
}

void AcousticManager::listenAroundCam () const {

    std::vector <Ray> rays = genRaysAroundCam(32);      // no need to generate every frame
}

std::vector <Ray> AcousticManager::genRaysAroundCam (int numRays) const {

    assert(numRays > 0);

    std::vector<Ray> rays;
    rays.reserve(numRays);

    float goldenRatio = (1.0f + std::sqrt(5.0f)) / 2.0f;    // may be could be constexpr since 26 standart

    for (int i = 0; i < numRays; ++i) {

        float theta = 2 * std::numbers::pi * i / goldenRatio;
        float phi = std::acos(1.0f - 2.0f * (i + 0.5f) / numRays);
        
        fc::Vector3f dir (

            std::cos(theta) * std::sin(phi),
            std::sin(theta) * std::sin(phi),
            std::cos(phi)
        );

        rays.emplace_back(Ray{camera_->position, dir});
    }

    return rays;
}