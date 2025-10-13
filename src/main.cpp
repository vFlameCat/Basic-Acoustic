#include <raylib.h>

#include "Game.hpp"
#include "SceneSoundPlayer.hpp"
#include "AcousticManager.hpp"


int main () {

    SampleManager samples[] = {

        SampleManager("../resources/7NA.mp3"),
        SampleManager("../resources/evil.mp3"),
        SampleManager("../resources/hateyou.mp3"),
        SampleManager("../resources/magister.mp3"),
        SampleManager("../resources/radioactive.mp3"),
        SampleManager("../resources/wellerman.mp3")
    };

    SceneSoundPlayer player;
    player.addSampleManager(&samples[0]);
    player.addSampleManager(&samples[1]);
    player.addSampleManager(&samples[2]);
    player.addSampleManager(&samples[3]);
    player.addSampleManager(&samples[4]);
    player.addSampleManager(&samples[5]);



    Game game(1280, 720);
    
    Box floorBox {

        .box = BoundingBox{Vector3{-20.f, -1.f, -20.f}, Vector3{20.f, 0.f, 20.f}},
        .color = LIGHTGRAY,
    };


    Box walls[] = {
        {
         .box = BoundingBox{Vector3{4.f, 0.f, 4.f}, Vector3{10.f, 10.f, 10.f}},
         .color = MAGENTA
        },
        {
         .box = BoundingBox{Vector3{-10.f, 0.f, 4.f}, Vector3{-4.f, 10.f, 10.f}},
         .color = MAGENTA
        },
        {
         .box = BoundingBox{Vector3{4.f, 0.f, -10.f}, Vector3{10.f, 10.f, -4.f}},
         .color = MAGENTA
        },
        {
         .box = BoundingBox{Vector3{-10.f, 0.f, -10.f}, Vector3{-4.f, 10.f, -4.f}},
         .color = MAGENTA
        },
    };
    

    Sphere sounds[] = {
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
            .center = Vector3{0.f, -2.f, 0.f},
            .radius = 1.f,
            .color  = SKYBLUE,
        },
        {
            .center = Vector3{12.f, 3.f, -7.f},
            .radius = 1.f,
            .color  = SKYBLUE,
        },
        {
            .center = Vector3{-7.f, 3.f, -12.f},
            .radius = 1.f,
            .color  = SKYBLUE,
        },
        {
            .center = Vector3{-12.f, 3.f, 7.f},
            .radius = 1.f,
            .color  = SKYBLUE,
        },
    };

    game.scene.addObject(&floorBox);
    game.scene.addObject(&walls[0]);
    game.scene.addObject(&walls[1]);
    game.scene.addObject(&walls[2]);
    game.scene.addObject(&walls[3]);

    game.scene.addObject(&sounds[0]);
    game.scene.addObject(&sounds[1]);
    game.scene.addObject(&sounds[2]);
    game.scene.addObject(&sounds[3]);
    game.scene.addObject(&sounds[4]);
    game.scene.addObject(&sounds[5]);


    game.acousticManager.addSoundSource(SoundSource{sounds[0].center, &samples[0]});
    game.acousticManager.addSoundSource(SoundSource{sounds[1].center, &samples[1]});
    game.acousticManager.addSoundSource(SoundSource{sounds[2].center, &samples[2]});
    game.acousticManager.addSoundSource(SoundSource{sounds[3].center, &samples[3]});
    game.acousticManager.addSoundSource(SoundSource{sounds[4].center, &samples[4]});
    game.acousticManager.addSoundSource(SoundSource{sounds[5].center, &samples[5]});


    game.run();

    return 0;
}