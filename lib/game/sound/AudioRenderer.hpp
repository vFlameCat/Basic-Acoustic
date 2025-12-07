#pragma once


#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <array>
#include <SyncPlayers.hpp>


class AudioRenderer final {

public:

    AudioRenderer ();

    void renderAudio (float *pOutput, uint32_t frameCount);

public:

    SyncStaticPlayCursors staticPlayCursors{};
    SyncDynamicPlayCursors dynamicPlayCursors{};

private:

    void render                   (float *pOutput, uint32_t frameCount);
    void renderStaticPlayCursors  (float *pOutput, uint32_t frameCount);
    void renderDynamicPlayCursors (float *pOutput, uint32_t frameCount);

    void renderWithoutAdvance                   (float *pOutput, uint32_t frameCount);
    void renderStaticPlayCursorsWithoutAdvance  (float *pOutput, uint32_t frameCount);
    void renderDynamicPlayCursorsWithoutAdvance (float *pOutput, uint32_t frameCount);

    static constexpr uint32_t OVERLAP_BUF_SIZE = 500;
    std::array <float, OVERLAP_BUF_SIZE> overlapBuf_{};
};