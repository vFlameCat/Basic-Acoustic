#pragma once


#include <raylib.h>
#include <iostream>

#include <vector>


class Scene final {

public:

    struct Box {

        BoundingBox bounds;
        Color color;
    };

    struct Sphere {

        Vector3 center;
        float radius;
        Color color;
    };

    Scene ();

    void addObject (Box box);
    void addObject (Sphere sphere);

    void drawScene () const;
    void drawSceneBoxes () const;
    void drawSceneSpheres () const;

    RayCollision getRayCollision (Ray ray) const;
    RayCollision getRayCollisionBoxes (Ray ray) const;
    RayCollision getRayCollisionSpheres (Ray ray) const;

private:

    std::vector <Box> boxes_;
    std::vector <Sphere> spheres_;
};  