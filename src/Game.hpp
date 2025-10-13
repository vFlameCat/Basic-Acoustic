#pragma once


#include <raylib.h>

#include "Scene.hpp"
#include "AcousticManager.hpp"


class Game final {

public:

    Game (int screenWidth, int screenHeight);

    Game (const Game&) = delete;
    Game& operator= (const Game&) = delete;

    ~Game ();

    void run ();

public:

    Scene scene;
    Camera camera;

    AcousticManager acousticManager;

private:

    void drawScene ();

private:

    const int screenWidth_, screenHeight_;
};