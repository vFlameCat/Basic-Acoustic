#pragma once


#include <initializer_list>
#include <string_view>
#include <vector>

#include "miniaudio.hpp"
#include "SampleManager.hpp"


class SceneSoundPlayer final {

public:

    SceneSoundPlayer ();

    SceneSoundPlayer (const SceneSoundPlayer&) = delete;
    SceneSoundPlayer& operator= (const SceneSoundPlayer&) = delete;

    ~SceneSoundPlayer ();

    void addSampleManager (SampleManager *manager);

    static void callbakcPlayer (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

private:

    void initDevice ();

private:

    std::vector <SampleManager*> sampleManagers_;

    ma_device device_;
};