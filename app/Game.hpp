#pragma once


#include <raylib.h>
#include "Scene.hpp"
#include <SimulationManager.hpp>


namespace rta {

class AudioEngine;

} // namespace rta


class Game final {

public:

    Game (int screenWidth, int screenHeight, rta::AudioEngine &engine);

    Game (const Game&) = delete;
    Game& operator= (const Game&) = delete;

    ~Game ();

    void run ();

public:

    Scene scene{};
    Camera camera{};

    rta::SimulationManager simulationManager;

private:

    void drawScene ();
    void drawDebugUI ();

private:

    rta::AudioEngine &engine_;

    const int screenWidth_, screenHeight_;
};