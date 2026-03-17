#include <AudioSourcesStorage.hpp>
#include <AudioStorage.hpp>
#include <Game.hpp>
#include <AudioPlayer.hpp>
#include <SimulationManager.hpp>
#include <SyncPlayers.hpp>

#include <raylib.h>

#include <vector>


int main () {    

    AudioStorage& audioStorage = AudioStorage::getInstance();

    std::vector <std::string> soundPaths = {
        APP_RESOURCES_ROOT "7NA.mp3",
        APP_RESOURCES_ROOT "hateyou.mp3",
        APP_RESOURCES_ROOT "magister.mp3",
    };

    std::vector <AudioStorage::Handle> audio;
    for (const auto& soundPath: soundPaths) {

        audio.push_back(audioStorage.loadAudio(soundPath));
    }

    Game game(1280, 720);

    std::vector <Scene::Box> boxes = {

        // floor
        {
         .bounds = BoundingBox{Vector3{-20.f, -1.f, -20.f}, Vector3{20.f, 0.f, 20.f}},
         .color = LIGHTGRAY
        },

        // walls
        {
         .bounds = BoundingBox{Vector3{4.f, 0.f, 4.f}, Vector3{10.f, 10.f, 10.f}},
         .color = MAGENTA
        },
        {
         .bounds = BoundingBox{Vector3{-10.f, 0.f, 4.f}, Vector3{-4.f, 10.f, 10.f}},
         .color = MAGENTA
        },
        {
         .bounds = BoundingBox{Vector3{4.f, 0.f, -10.f}, Vector3{10.f, 10.f, -4.f}},
         .color = MAGENTA
        },
        {
         .bounds = BoundingBox{Vector3{-10.f, 0.f, -10.f}, Vector3{-4.f, 10.f, -4.f}},
         .color = MAGENTA
        },

        

        // пол второго этажа
        {
            .bounds = BoundingBox{Vector3{-20.f, -9.f, -20.f}, Vector3{20.f, -10.f, 20.f}},
            .color = GRAY
        },

        // стены второго этажа
        // передняя стена
        {
            .bounds = BoundingBox{Vector3{-20.f, -10.f, -20.f}, Vector3{20.f, -1.f, -19.f}},
            .color = DARKGRAY
        },
        // задняя стена
        {
            .bounds = BoundingBox{Vector3{-20.f, -10.f, 19.f}, Vector3{20.f, -1.f, 20.f}},
            .color = DARKGRAY
        },
        // левая стена
        {
            .bounds = BoundingBox{Vector3{-20.f, -10.f, -19.f}, Vector3{-19.f, -1.f, 19.f}},
            .color = DARKGRAY
        },
        // правая стена
        {
            .bounds = BoundingBox{Vector3{19.f, -10.f, -19.f}, Vector3{20.f, -1.f, 19.f}},
            .color = DARKGRAY
        },
    };
    

    std::vector <Scene::Sphere> spheres = {
        {
            .center = Vector3{0.f, 1.f, 0.f},
            .radius = 1.f,
            .color  = SKYBLUE,
        },
        {
            .center = Vector3{7.f, 3.f, 12.f},
            .radius = 1.f,
            .color  = SKYBLUE,
        },
        {
            .center = Vector3{-7.f, 3.f, -12.f},
            .radius = 1.f,
            .color  = SKYBLUE,
        },
        {
            .center = Vector3{0.f, -2.f, 0.f},
            .radius = 1.f,
            .color  = SKYBLUE,
        },
        // {
        //     .center = Vector3{12.f, 3.f, -7.f},
        //     .radius = 1.f,
        //     .color  = SKYBLUE,
        // },
        // {
        //     .center = Vector3{-12.f, 3.f, 7.f},
        //     .radius = 1.f,
        //     .color  = SKYBLUE,
        // },
    };

    
    for (auto box: boxes) {

        game.scene.addObject(box);
    }

    for (auto sphere: spheres) {

        game.scene.addObject(sphere);
    }


    AudioPlayer &player = AudioPlayer::getInstance();
    std::vector <SyncStaticPlayCursors::Handle> playerHandles;
    for (auto audioHandle: audio) {

        SyncStaticPlayCursors::Handle handle = player.getStaticPlayCursors().addPlayCursor(PlayCursor::CreateInfo{.volume = 0.f, .audio = audioStorage.getAudio(audioHandle)});
        playerHandles.push_back(handle);
    }

    for (size_t i = 0; i < audio.size(); ++i) {

        game.simulationManager.audioSources.addAudioSource(AudioSource{spheres[i].center, playerHandles[i]});
    }
    game.simulationManager.audioSources.addAudioSource(AudioSource{spheres[3].center, playerHandles[0]});



    player.startPlayer();
    game.run();
}