#pragma once


#include <cstdint>
#include <array>
#include <SyncPlayers.hpp>


class AudioRenderer final {

public:

    AudioRenderer ();

    void renderAudio (float *pOutput, uint32_t frameCount);

public:

    SyncStaticPlayCursors staticPlayCursors;
    SyncDynamicPlayCursors dynamicPlayCursors;

private:

    void render             (float *pOutput, uint32_t frameCount, std::vector<PlayCursor> &staticPlayers, std::vector<PlayCursor> &dynamicPlayers);
    void renderPlayCursors  (float *pOutput, uint32_t frameCount, std::vector<PlayCursor> &players);

    void renderWithoutAdvance             (float *pOutput, uint32_t frameCount, std::vector<PlayCursor> &staticPlayers, std::vector<PlayCursor> &dynamicPlayers);
    void renderPlayCursorsWithoutAdvance  (float *pOutput, uint32_t frameCount, std::vector<PlayCursor> &players);

    static constexpr uint32_t OVERLAP_BUF_SIZE = 500;
    std::array <float, OVERLAP_BUF_SIZE> overlapBuf_{};
};