#pragma once


#include <vector>

#include "Scene.hpp"
#include "SampleManager.hpp"


struct SoundSource {

    Vector3 position;
    SampleManager *manager;
};  


class AcousticManager final {

public:

    AcousticManager (const Scene *scene, const Camera *camera);

    AcousticManager (const AcousticManager&) = default;
    AcousticManager& operator= (const AcousticManager&) = default;

    ~AcousticManager () = default;

    void addSoundSource (SoundSource source);
    void checkSourcesVisibility ();

private:

    const Scene *scene_;
    const Camera *camera_;

    std::vector <SoundSource> sources_;
};