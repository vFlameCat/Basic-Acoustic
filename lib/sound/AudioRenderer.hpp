#pragma once


#include <cstdint>
#include <array>
#include <vector>

#include <SyncPlayers.hpp>
#include <containers/SlotPool.hpp>


class AudioRenderer final {

public:

    AudioRenderer ();

    void renderAudio (float *pOutput, uint32_t frameCount);

public:

    SyncStaticPlayCursors  staticPlayCursors;
    SyncDynamicPlayCursors dynamicPlayCursors;

private:

    void render (float *pOutput, uint32_t frameCount, fc::SlotPool<PlayCursor> &staticPlayers, std::vector<PlayCursor> &dynamicPlayers);
    void renderWithoutAdvance (float *pOutput, uint32_t frameCount, fc::SlotPool<PlayCursor> &staticPlayers, std::vector<PlayCursor> &dynamicPlayers);

    template <typename Container>
    void renderPlayCursors (float *pOutput, uint32_t frameCount, Container &players);

    template <typename Container>
    void renderPlayCursorsWithoutAdvance (float *pOutput, uint32_t frameCount, Container &players);

    static constexpr uint32_t OVERLAP_BUF_SIZE = 500;
    std::array <float, OVERLAP_BUF_SIZE> overlapBuf_{};
};
