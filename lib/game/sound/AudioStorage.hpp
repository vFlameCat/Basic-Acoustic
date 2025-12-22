#pragma once


#include <miniaudio.hpp>

#include <string>
#include <vector>
#include <cstdint>


class Audio {

public:

    using size_type = std::vector<float>::size_type;


    Audio () = default;
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


    AudioStorage (const AudioStorage&) = delete;
    AudioStorage& operator= (const AudioStorage&) = delete;
    

    static AudioStorage& getInstance ();

    [[nodiscard]] Handle loadAudio (const std::string &samplePath);
    void unloadAudio (Handle handle);
    
    const Audio& getAudio (Handle handle) const;

private:

    AudioStorage () = default;

    int decodeAndLoadFloatSamples (const std::string &samplePath, std::vector <float> &samples) const;

private:

    std::vector<Audio> storage_;
};