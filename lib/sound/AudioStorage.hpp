#pragma once


#include <miniaudio.hpp>

#include <containers/SlotPool.hpp>

#include <string>
#include <vector>
#include <memory>


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

    using Handle = fc::SlotPool<std::unique_ptr<Audio>>::Handle;


    AudioStorage () = default;

    AudioStorage (const AudioStorage&) = delete;
    AudioStorage& operator= (const AudioStorage&) = delete;


    [[nodiscard]] Handle load (const std::string &samplePath);
    void unload (Handle handle);

    const Audio& get (Handle handle) const;

private:

    int decodeAndLoadFloatSamples (const std::string &samplePath, std::vector <float> &samples) const;

private:

    fc::SlotPool<std::unique_ptr<Audio>> storage_{};
};
