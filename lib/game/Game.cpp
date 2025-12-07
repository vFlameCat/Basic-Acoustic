#include "Game.hpp"


Game::Game (int screenWidth, int screenHeight):
  acousticManager(&scene, &camera),
  screenWidth_(screenWidth),
  screenHeight_(screenHeight) {

    InitWindow(screenWidth_, screenHeight_, "Test");

    camera = Camera {

        .position = Vector3{-3.f, -2.f, 0.f},
        .target   = Vector3{0.f, -2.f, 0.f},
        .up       = Vector3{0.f, 1.f, 0.f},
        .fovy     = 90.f,

        .projection = CAMERA_PERSPECTIVE,
    };
}

Game::~Game () {

    CloseWindow(); 
}

void Game::run () {

    DisableCursor(); 

    while (!WindowShouldClose()) {

        UpdateCamera(&camera, CAMERA_FREE);

        acousticManager.listenAroundCam();

        drawScene();
    }
}

void Game::drawScene () {

    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

            scene.drawScene();

        EndMode3D();

    EndDrawing();
}