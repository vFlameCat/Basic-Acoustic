#include "AcousticManager.hpp"

#include <cmath>


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
  camera_(camera),
  sources_() {

    if (!scene_ || !camera_) {

        std::cerr << "Error! Invalid Arguments in AcousticManager Ctor\n";
    }
}

void AcousticManager::addSoundSource (SoundSource source) {

    if (!source.manager) {

        std::cerr << "Error! Invalid manager ptr in addSoundSource!\n";
        return;
    }

    sources_.push_back(source);
}

void AcousticManager::checkSourcesVisibility () {

    for (auto &source: sources_) {

        float distanceToSource = getDistance(camera_->position, source.position);

        Vector3 rayDir = Vector3{source.position.x - camera_->position.x,
                                 source.position.y - camera_->position.y,
                                 source.position.z - camera_->position.z};

        RayCollision collision = scene_->getRayCollisionBoxes(Ray(camera_->position, normalize(rayDir)));

        if (collision.hit && collision.distance < distanceToSource) {

            source.manager->volume = 0.f;
        }
        else {

            source.manager->volume = 1.f;
        }
    }
}