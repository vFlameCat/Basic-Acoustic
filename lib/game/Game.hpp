#pragma once


#include <raylib.h>
#include <Scene.hpp>
#include <SimulationManager.hpp>


class AudioEngine;


class Game final {

public:

    Game (int screenWidth, int screenHeight, AudioEngine &engine);

    Game (const Game&) = delete;
    Game& operator= (const Game&) = delete;

    ~Game ();

    void run ();

public:

    Scene scene{};
    Camera camera{};

    SimulationManager simulationManager;

private:

    void drawScene ();
    void drawDebugUI ();

private:

    AudioEngine &engine_;

    const int screenWidth_, screenHeight_;
};