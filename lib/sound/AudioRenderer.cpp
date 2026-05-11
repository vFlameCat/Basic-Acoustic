#include <Players/Player.hpp>
#include <Players/PlayersPool.hpp>
#include <Players/SpatialFramePlayers.hpp>
#include <AudioRenderer.hpp>

#include <algorithm>
#include <cmath>
#include <vector>


AudioRenderer::AudioRenderer ():
  overlapBuf_(INITIAL_OVERLAP_BUF_SIZE, 0.f) {}

void AudioRenderer::renderAudio (float *pOutput, uint32_t frameCount) {

    // The order of acquiring frame reader and pool reader is important.
    // Main thread firstly updates playersPool and then spatialFramePlayers,
    // so to avoid data race in audio thread we need to acquire spatial frame first.
    SpatialFramePlayers::Reader spatialReader = spatialFramePlayers.getReader();
    PlayersPool::Reader poolReader  = playersPool.getReader();
    spatialReader.buildPlayers(poolReader);

    fc::SlotPool<Player> &poolPlayers  = poolReader.getPlayers();
    std::vector<Player>  &spatialPlayers = spatialReader.getPlayers();

    mixPlayers(pOutput, frameCount, poolPlayers, spatialPlayers);

    const uint32_t overlapCount = frameCount / 2;
    if (overlapBuf_.size() < overlapCount) {

        overlapBuf_.resize(overlapCount, 0.f);
    }

    for (uint32_t i = 0; i < overlapCount; ++i) {

        float fadeIn = static_cast<float>(i) / overlapCount;
        float fadeOut = 1.f - fadeIn;

        pOutput[i] = overlapBuf_[i] * fadeOut + pOutput[i] * fadeIn;
    }

    mixPlayersWithoutAdvance(overlapBuf_.data(), overlapCount, poolPlayers, spatialPlayers);

    // Soft clip via tanh: keeps the summed signal in (-1, 1) so the driver
    // never hard-clips on coherent peaks across many spatial reflections.
    for (uint32_t i = 0; i < frameCount; ++i) {

        pOutput[i] = std::tanh(pOutput[i]);
    }
}

void AudioRenderer::mixPlayers (float *pOutput, uint32_t frameCount, fc::SlotPool<Player> &poolPlayers, std::vector<Player> &spatialPlayers) {

    std::fill_n(pOutput, frameCount, 0.f);

    mix(pOutput, frameCount, poolPlayers);
    mix(pOutput, frameCount, spatialPlayers);
}

template <typename Container>
void AudioRenderer::mix (float *pOutput, uint32_t frameCount, Container &players) {

    for (auto& player: players) {

        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += player.getSample();
            player.advance();
        }
    }
}

void AudioRenderer::mixPlayersWithoutAdvance (float *pOutput, uint32_t frameCount, fc::SlotPool<Player> &poolPlayers, std::vector<Player> &spatialPlayers) {

    std::fill_n(pOutput, frameCount, 0.f);

    mixWithoutAdvance(pOutput, frameCount, poolPlayers);
    mixWithoutAdvance(pOutput, frameCount, spatialPlayers);
}

template <typename Container>
void AudioRenderer::mixWithoutAdvance (float *pOutput, uint32_t frameCount, Container &players) {

    for (auto& player: players) {

        double originalPos = player.pos_;
        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += player.getSample();
            player.advance();
        }
        player.pos_ = originalPos;
    }
}
