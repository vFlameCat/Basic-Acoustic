#pragma once


#include "SyncPlayers.hpp"
#include <Vector3.hpp>
#include <AudioStorage.hpp>

#include <cstdint>
#include <vector>


struct AudioSource {

    fc::Vector3f position = fc::Vector3f(0.f, 0.f,  0.f);
    SyncStaticPlayCursors::Handle handle = SyncStaticPlayCursors::Handle::Invalid;

    bool isValid () const { return handle != SyncStaticPlayCursors::Handle::Invalid; }
};


class AudioSourcesStorage final {

public:

    enum class Handle: uint32_t {

        Invalid = uint32_t(-1)
    };


    AudioSourcesStorage () = default;

    Handle addAudioSource (const AudioSource &source);

    void removeAudioSource (Handle handle);

    std::vector<AudioSource>::iterator begin() { return storage_.begin(); }
    std::vector<AudioSource>::iterator end() { return storage_.end(); }
    
    std::vector<AudioSource>::const_iterator begin() const { return storage_.begin(); }
    std::vector<AudioSource>::const_iterator end() const { return storage_.end(); }

    std::vector<AudioSource>::const_iterator cbegin() const { return storage_.cbegin(); }
    std::vector<AudioSource>::const_iterator cend() const { return storage_.cend(); }

private:

    std::vector<AudioSource> storage_{};
};