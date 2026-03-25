#include "PlayCursor.hpp"
#include "SyncPlayers.hpp"
#include <AudioRenderer.hpp>

#include <cstring>
#include <vector>


AudioRenderer::AudioRenderer () {

    overlapBuf_.fill(0.f);
}

void AudioRenderer::renderAudio (float *pOutput, uint32_t frameCount) {

    SyncStaticPlayCursors::RenderView renderView = staticPlayCursors.getRenderView();
    SyncDynamicPlayCursors::FrameRenderer frameRenderer = dynamicPlayCursors.getFrameRenderer(renderView);

    std::vector<PlayCursor> &staticPlayers = renderView.getPlayCursors();
    std::vector<PlayCursor> &dynamicPlayers = frameRenderer.getPlayCursors();

    render(pOutput, frameCount, staticPlayers, dynamicPlayers);

    const uint32_t overlapCount = frameCount / 2;
    for (uint32_t i = 0; i < overlapCount; ++i) {

        float fadeIn = static_cast<float>(i) / overlapCount;
        float fadeOut = 1.f - fadeIn;

        pOutput[i] = overlapBuf_[i] * fadeOut + pOutput[i] * fadeIn;
    }

    renderWithoutAdvance(overlapBuf_.data(), overlapCount, staticPlayers, dynamicPlayers);
}

void AudioRenderer::render (float *pOutput, uint32_t frameCount, std::vector<PlayCursor> &staticPlayers, std::vector<PlayCursor> &dynamicPlayers) {

    memset(pOutput, 0, frameCount * sizeof(float));

    renderPlayCursors(pOutput, frameCount, staticPlayers);
    renderPlayCursors(pOutput, frameCount, dynamicPlayers);
}

void AudioRenderer::renderPlayCursors (float *pOutput, uint32_t frameCount, std::vector<PlayCursor> &players) {

    for (auto& playCursor: players) {

        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += playCursor.getSample();
            playCursor.advance();
        }
    }
}

void AudioRenderer::renderWithoutAdvance (float *pOutput, uint32_t frameCount, std::vector<PlayCursor> &staticPlayers, std::vector<PlayCursor> &dynamicPlayers) {

    memset(pOutput, 0, frameCount * sizeof(float));

    renderPlayCursorsWithoutAdvance(pOutput, frameCount, staticPlayers);
    renderPlayCursorsWithoutAdvance(pOutput, frameCount, dynamicPlayers);
}

void AudioRenderer::renderPlayCursorsWithoutAdvance (float *pOutput, uint32_t frameCount, std::vector<PlayCursor> &players) {

    for (auto& playCursor: players) {

        double originalPos = playCursor.pos_;
        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += playCursor.getSample();
            playCursor.advance();
        }
        playCursor.pos_ = originalPos;
    }
}