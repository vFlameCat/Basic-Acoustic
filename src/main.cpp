#include <raylib.h>
#include <vector>

#include "Game.hpp"
#include "SoundPlayer.hpp"
#include "AcousticManager.hpp"
#include "math.hpp"


int main () {

    SoundStorage& soundStorage = SoundStorage::getInstance();
    SoundPlayer &player = SoundPlayer::getInstance();

    std::vector <std::string> soundPaths = {
        "../resources/7NA.mp3",
        "../resources/hateyou.mp3",
        "../resources/magister.mp3",
    };
    

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
        }
    };
    

    std::vector <Scene::Sphere> spheres = {
        {
            .center = Vector3{0.f, 1.f, 0.f},
            .radius = 1.f,
            .color  = SKYBLUE,
        },
        // {
        //     .center = Vector3{0.f, -2.f, 0.f},
        //     .radius = 1.f,
        //     .color  = SKYBLUE,
        // },
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


    std::vector <SoundStorage::SoundHandle> samples;
    for (const auto& soundPath: soundPaths) {

        SoundStorage::SoundHandle handle = soundStorage.loadSound(soundPath); 

        samples.push_back(handle);
        player.addStaticPlayer(PlayCursor::CreateInfo{.volume = 0.f, .soundHandle = handle});  
    }

    for (size_t i = 0; i < samples.size(); ++i) {

        game.acousticManager.addSoundSource(AcousticManager::SoundSource{spheres[i].center, samples[i]});
    }



    player.startPlayer();
    game.run();
}