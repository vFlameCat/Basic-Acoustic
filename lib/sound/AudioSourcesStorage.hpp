#pragma once


#include "SyncPlayers.hpp"
#include <Vector3.hpp>
#include <AudioStorage.hpp>

#include <containers/SlotPool.hpp>


struct AudioSource {

    fc::Vector3f position = fc::Vector3f(0.f, 0.f,  0.f);
    SyncStaticPlayCursors::Handle handle = SyncStaticPlayCursors::Handle::Invalid;
};


using AudioSourcesStorage = fc::SlotPool<AudioSource>;
