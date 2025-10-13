#pragma once


#include <raylib.h>
#include <iostream>

#include <vector>


struct Box {

    BoundingBox box;
    Color color;
};

struct Sphere {

    Vector3 center;
    float radius;
    Color color;
};


class Scene final {

public:

    Scene ();

    void addObject (const Box *box);
    void addObject (const Sphere *sphere);

    void drawScene () const;
    void drawSceneBoxes () const;
    void drawSceneSpheres () const;

    RayCollision getRayCollision (Ray ray) const;
    RayCollision getRayCollisionBoxes (Ray ray) const;
    RayCollision getRayCollisionSpheres (Ray ray) const;

private:

    std::vector <const Box*> boxes_;
    std::vector <const Sphere*> spheres_;
};  