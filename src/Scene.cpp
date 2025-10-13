#include "Scene.hpp"


Scene::Scene ():
  boxes_(), spheres_() {}

void Scene::addObject (const Box *box) {

    if (!box) {

        std::cerr << "Warning! You have passed nullptr to addObject!\n";
        return;
    }

    boxes_.push_back(box);
}

void Scene::addObject (const Sphere *sphere) {

    if (!sphere) {

        std::cerr << "Warning! You have passed nullptr to addObject!\n";
        return;
    }

    spheres_.push_back(sphere);
}

void Scene::drawScene () const {

    drawSceneBoxes();
    drawSceneSpheres();
}

void Scene::drawSceneBoxes () const {

    for (size_t i = 0, size = boxes_.size(); i < size; ++i) {

        const BoundingBox &box = boxes_[i]->box;

        Vector3 boxCenter = {

            (box.min.x + box.max.x) / 2,
            (box.min.y + box.max.y) / 2,
            (box.min.z + box.max.z) / 2
        };

        Vector3 boxSize = {

            box.max.x - box.min.x,
            box.max.y - box.min.y,
            box.max.z - box.min.z
        };

        DrawCubeV(boxCenter, boxSize, boxes_[i]->color);
        DrawCubeWiresV(boxCenter, boxSize, BLACK);
    }
}

void Scene::drawSceneSpheres () const {

    for (size_t i = 0, size = spheres_.size(); i < size; ++i) {

        DrawSphere(spheres_[i]->center, spheres_[i]->radius, spheres_[i]->color);
        DrawSphereWires(spheres_[i]->center, spheres_[i]->radius, 16, 16, BLACK);
    }
}

RayCollision Scene::getRayCollision (Ray ray) const {

    RayCollision collision = getRayCollisionBoxes(ray);
    RayCollision spheresCollision = getRayCollisionSpheres(ray);

    if (spheresCollision.hit && \
        (!collision.hit || spheresCollision.distance < collision.distance)) {

        collision = spheresCollision;
    }

    return collision;
}

RayCollision Scene::getRayCollisionBoxes (Ray ray) const {

    RayCollision collision = {};

    for (size_t i = 0, size = boxes_.size(); i < size; ++i) {

        RayCollision curCollision = GetRayCollisionBox(ray, boxes_[i]->box);
        if (curCollision.hit && \
            (!collision.hit || curCollision.distance < collision.distance)) {

            collision = curCollision;
        }
    }

    return collision;
}

RayCollision Scene::getRayCollisionSpheres (Ray ray) const {

    RayCollision collision = {};

    for (size_t i = 0, size = spheres_.size(); i < size; ++i) {

        RayCollision curCollision = GetRayCollisionSphere(ray, spheres_[i]->center, spheres_[i]->radius);
        if (curCollision.hit && \
            (!collision.hit || curCollision.distance < collision.distance)) {

            collision = curCollision;
        }
    }

    return collision;
}