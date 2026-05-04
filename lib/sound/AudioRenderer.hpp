#pragma once


#include <cstdint>
#include <array>
#include <vector>

#include <Players/PlayersPool.hpp>
#include <Players/SpatialFramePlayers.hpp>

#include <containers/SlotPool.hpp>


class AudioRenderer final {

public:

    AudioRenderer ();

    void renderAudio (float *pOutput, uint32_t frameCount);

public:

    PlayersPool         playersPool;
    SpatialFramePlayers spatialFramePlayers;

private:

    void render (float *pOutput, uint32_t frameCount, fc::SlotPool<Player> &poolPlayers, std::vector<Player> &framePlayers);
    void renderWithoutAdvance (float *pOutput, uint32_t frameCount, fc::SlotPool<Player> &poolPlayers, std::vector<Player> &framePlayers);

    template <typename Container>
    void renderPlayers (float *pOutput, uint32_t frameCount, Container &players);

    template <typename Container>
    void renderPlayersWithoutAdvance (float *pOutput, uint32_t frameCount, Container &players);

    static constexpr uint32_t OVERLAP_BUF_SIZE = 500;
    std::array <float, OVERLAP_BUF_SIZE> overlapBuf_{};
};
