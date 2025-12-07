#pragma once


#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "miniaudio.hpp"


class SoundStorage final {

public:

    using SoundHandle = std::uint64_t;
    static constexpr SoundHandle INVALID_SOUND_HANDLE = 0;

    class Sound {

    public:

        Sound (std::vector <float> &&samples);
        
        // it is correct to take pos + 1 for any correct pos! (for looped sound processing performance)
        // the last sample is equal to the first
        float operator[] (std::size_t pos) const;
        float at (std::size_t pos) const;

        //returns sample's real size
        std::size_t size () const;

    private:

        std::vector <float> samples_;
    };

    SoundStorage (const SoundStorage&) = delete;
    SoundStorage& operator= (const SoundStorage&) = delete;

    static SoundStorage& getInstance ();

    [[nodiscard]] SoundHandle loadSound (const std::string &samplePath);
    void unloadSound (SoundHandle soundHandle);
    const Sound& getSound (SoundHandle soundHandle) const;

private:

    SoundStorage () = default;

    int decodeAndLoadFloatSamples (const std::string &samplePath, std::vector <float> &samples) const;

private:

    SoundHandle nextHandle_ = 1;

    std::unordered_map <SoundHandle, Sound> sounds_{};
};