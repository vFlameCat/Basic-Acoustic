#pragma once


#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include "miniaudio.hpp"


class SampleManager final {

public:

    SampleManager (std::string_view samplePath);

    SampleManager (const SampleManager&) = delete;
    SampleManager& operator= (const SampleManager&) = delete;

    ~SampleManager ();

    float getSample ();
    void advance ();

public:

    float pitch, volume;

private:

    void initDecoder (std::string_view samplePath);
    void loadSamples ();

private:

    double advance_;

    std::vector<float> samples_;

    ma_decoder decoder_;
};