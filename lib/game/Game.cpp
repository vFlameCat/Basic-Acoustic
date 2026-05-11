#include "Game.hpp"
#include "SimulationManager.hpp"
#include "AudioEngine.hpp"

#include <rlImGui.h>
#include <imgui.h>


Game::Game (int screenWidth, int screenHeight, AudioEngine &engine):
  simulationManager(engine),
  engine_(engine),
  screenWidth_(screenWidth),
  screenHeight_(screenHeight) {

    InitWindow(screenWidth_, screenHeight_, "Test");

    rlImGuiSetup(true);

    camera = Camera {

        .position = Vector3{-3.f, -2.f, 0.f},
        .target   = Vector3{0.f, -2.f, 0.f},
        .up       = Vector3{0.f, 1.f, 0.f},
        .fovy     = 90.f,

        .projection = CAMERA_PERSPECTIVE,
    };
}

Game::~Game () {

    rlImGuiShutdown();
    CloseWindow();
}

void Game::run () {

    DisableCursor();

    while (!WindowShouldClose()) {

        const bool cursorActive = IsKeyDown(KEY_TAB);

        if (cursorActive && IsCursorHidden())    EnableCursor();
        if (!cursorActive && !IsCursorHidden())  DisableCursor();

        // Freeze camera look while the cursor is being used for ImGui interaction.
        if (!cursorActive) {

            UpdateCamera(&camera, CAMERA_FREE);
        }

        simulationManager.listener.position = camera.position;
        simulationManager.listenAroundCam([&scene = this->scene](Ray ray) { return scene.getRayCollisionBoxes(ray); });

        drawScene();
    }
}

void Game::drawScene () {

    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

            scene.drawScene();

        EndMode3D();

        drawDebugUI();

    EndDrawing();
}

void Game::drawDebugUI () {

    rlImGuiBegin();

        ImGui::Begin("Audio debug");

            using fms = std::chrono::duration<double, std::milli>;

            const double lastMs = fms{engine_.getLastCallbackDuration()}.count();
            const double avgMs  = fms{engine_.getAvgCallbackDuration() }.count();

            ImGui::Text("Callback:     %.2f ms", lastMs);
            ImGui::Text("Avg (last %d): %.2f ms", 64, avgMs);

        ImGui::End();

    rlImGuiEnd();
}
