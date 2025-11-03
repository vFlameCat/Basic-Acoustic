#include <raylib.h>
#include <vector>

#include "Game.hpp"
#include "SoundPlayer.hpp"
#include "AcousticManager.hpp"
#include "math.hpp"


int main () {    

    SoundStorage& soundStorage = SoundStorage::getInstance();

    std::vector <std::string> soundPaths = {
        "/home/cat/Coding/Basic-Acoustic/resources/7NA.mp3",
        "/home/cat/Coding/Basic-Acoustic/resources/hateyou.mp3",
        "/home/cat/Coding/Basic-Acoustic/resources/magister.mp3",
    };

    std::vector <SoundStorage::SoundHandle> samples;
    for (const auto& soundPath: soundPaths) {

        samples.push_back(soundStorage.loadSound(soundPath));
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


    SoundPlayer &player = SoundPlayer::getInstance();
    std::vector <SoundPlayer::PlayerHandle> playerHandles;
    for (auto handle: samples) {

        SoundPlayer::PlayerHandle playerH = player.staticPlayers.addPlayer(PlayCursor::CreateInfo{.volume = 0.f, .soundHandle = handle});  
        playerHandles.push_back(playerH);
    }

    for (size_t i = 0; i < samples.size(); ++i) {

        game.acousticManager.addSoundSource(AcousticManager::SoundSource{spheres[i].center, playerHandles[i]});
    }
    game.acousticManager.addSoundSource(AcousticManager::SoundSource{spheres[3].center, playerHandles[0]});



    player.startPlayer();
    game.run();
}