#pragma once


#include <Players/PlayersPool.hpp>
#include <Vector3.hpp>

#include <containers/SlotPool.hpp>


struct AudioSource {

    fc::Vector3f position = fc::Vector3f(0.f, 0.f,  0.f);
    PlayersPool::Handle handle = PlayersPool::Handle::Invalid;
};


using AudioSourcesStorage = fc::SlotPool<AudioSource>;
