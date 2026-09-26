#pragma once


#include <Players/PlayersPool.hpp>
#include <Vector3.hpp>

#include <containers/SlotPool.hpp>


namespace rta {


struct AudioSource {

    Vector3f position = Vector3f(0.f, 0.f,  0.f);
    PlayersPool::Handle handle = PlayersPool::Handle::Invalid;
};


using AudioSourcesStorage = SlotPool<AudioSource>;


} // namespace rta
