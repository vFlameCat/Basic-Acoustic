#pragma once


#include <cstdint>
#include <vector>

#include <Players/PlayersPool.hpp>
#include <Players/SpatialFramePlayers.hpp>

#include <containers/SlotPool.hpp>


class AudioRenderer final {

public:

    AudioRenderer ();

    void renderAudio (float *pOutput, uint32_t frameCount);

public:

    PlayersPool         playersPool {};
    SpatialFramePlayers spatialFramePlayers {};

private:

    void mixPlayers (float *pOutput, uint32_t frameCount, fc::SlotPool<Player> &poolPlayers, std::vector<Player> &framePlayers);
    void mixPlayersWithoutAdvance (float *pOutput, uint32_t frameCount, fc::SlotPool<Player> &poolPlayers, std::vector<Player> &framePlayers);

    template <typename Container>
    void mix (float *pOutput, uint32_t frameCount, Container &players);

    template <typename Container>
    void mixWithoutAdvance (float *pOutput, uint32_t frameCount, Container &players);

    static constexpr uint32_t INITIAL_OVERLAP_BUF_SIZE = 1024;
    std::vector<float> overlapBuf_;
};
