#include <Players/Player.hpp>
#include <Players/PlayersPool.hpp>
#include <Players/SpatialFramePlayers.hpp>
#include <AudioRenderer.hpp>

#include <cstring>
#include <vector>


AudioRenderer::AudioRenderer () {

    overlapBuf_.fill(0.f);
}

void AudioRenderer::renderAudio (float *pOutput, uint32_t frameCount) {

    // The order of acquiring frame reader and pool reader is important.
    // Main thread firstly updates playersPool and then spatialFramePlayers,
    // so to avoid data race in audio thread we need to acquire spatial frame first.
    SpatialFramePlayers::Reader spatialReader = spatialFramePlayers.getReader();
    PlayersPool::Reader poolReader  = playersPool.getReader();
    spatialReader.buildPlayers(poolReader);

    fc::SlotPool<Player> &poolPlayers  = poolReader.getPlayers();
    std::vector<Player>  &spatialPlayers = spatialReader.getPlayers();

    render(pOutput, frameCount, poolPlayers, spatialPlayers);

    const uint32_t overlapCount = frameCount / 2;
    for (uint32_t i = 0; i < overlapCount; ++i) {

        float fadeIn = static_cast<float>(i) / overlapCount;
        float fadeOut = 1.f - fadeIn;

        pOutput[i] = overlapBuf_[i] * fadeOut + pOutput[i] * fadeIn;
    }

    renderWithoutAdvance(overlapBuf_.data(), overlapCount, poolPlayers, spatialPlayers);
}

void AudioRenderer::render (float *pOutput, uint32_t frameCount, fc::SlotPool<Player> &poolPlayers, std::vector<Player> &spatialPlayers) {

    memset(pOutput, 0, frameCount * sizeof(float));

    renderPlayers(pOutput, frameCount, poolPlayers);
    renderPlayers(pOutput, frameCount, spatialPlayers);
}

template <typename Container>
void AudioRenderer::renderPlayers (float *pOutput, uint32_t frameCount, Container &players) {

    for (auto& player: players) {

        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += player.getSample();
            player.advance();
        }
    }
}

void AudioRenderer::renderWithoutAdvance (float *pOutput, uint32_t frameCount, fc::SlotPool<Player> &poolPlayers, std::vector<Player> &spatialPlayers) {

    memset(pOutput, 0, frameCount * sizeof(float));

    renderPlayersWithoutAdvance(pOutput, frameCount, poolPlayers);
    renderPlayersWithoutAdvance(pOutput, frameCount, spatialPlayers);
}

template <typename Container>
void AudioRenderer::renderPlayersWithoutAdvance (float *pOutput, uint32_t frameCount, Container &players) {

    for (auto& player: players) {

        double originalPos = player.pos_;
        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += player.getSample();
            player.advance();
        }
        player.pos_ = originalPos;
    }
}
