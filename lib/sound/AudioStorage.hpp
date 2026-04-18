#pragma once


#include <miniaudio.hpp>

#include <string>
#include <vector>
#include <memory>
#include <cstdint>


class Audio {

public:

    using size_type = std::vector<float>::size_type;


    Audio (std::vector <float> &&samples);

    
    // it is correct to take pos + 1 for any correct pos! (for looped sound processing performance)
    // the last sample is equal to the first
    float operator[] (std::size_t pos) const;
    float at (std::size_t pos) const;

    //returns sample's real size
    size_type size () const;

private:

    std::vector <float> samples_;
};


class AudioStorage final {

public:

    enum class Handle: uint32_t {

        Invalid = uint32_t(-1)
    };


    AudioStorage () = default;

    AudioStorage (const AudioStorage&) = delete;
    AudioStorage& operator= (const AudioStorage&) = delete;


    [[nodiscard]] Handle loadAudio (const std::string &samplePath);
    void unloadAudio (Handle handle);

    const Audio& getAudio (Handle handle) const;

private:

    int decodeAndLoadFloatSamples (const std::string &samplePath, std::vector <float> &samples) const;

private:

    std::vector<std::unique_ptr<Audio>> storage_{};
};