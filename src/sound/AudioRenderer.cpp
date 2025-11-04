#include <AudioRenderer.hpp>
#include <cstring>


AudioRenderer::AudioRenderer () {

    overlapBuf_.fill(0.f);
}

void AudioRenderer::renderAudio (float *pOutput, uint32_t frameCount) {

    dynamicPlayCursors.recieve();

    const uint32_t overlapCount = frameCount / 2;

    render(pOutput, frameCount);

    for (uint32_t i = 0; i < overlapCount; ++i) {

        float fadeIn = static_cast<float>(i) / overlapCount;
        float fadeOut = 1.f - fadeIn;

        pOutput[i] = overlapBuf_[i] * fadeOut + pOutput[i] * fadeIn;
    }

    renderWithoutAdvance(overlapBuf_.data(), overlapCount);
}

void AudioRenderer::render (float *pOutput, uint32_t frameCount) {

    memset(pOutput, 0.f, frameCount * sizeof(float));

    renderStaticPlayCursors(pOutput, frameCount);
    renderDynamicPlayCursors(pOutput, frameCount);
}

void AudioRenderer::renderStaticPlayCursors (float *pOutput, uint32_t frameCount) {

    std::lock_guard<std::mutex> sync(staticPlayCursors.playCursorsSync_);

    for (auto& [handle, playCursor]: staticPlayCursors.playCursors_) {

        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += playCursor.getSample();
            playCursor.advance();
        }
    }
}

void AudioRenderer::renderDynamicPlayCursors (float *pOutput, uint32_t frameCount) {

    for (auto& playCursor: dynamicPlayCursors.playCursors_) {

        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += playCursor.getSample();
            playCursor.advance();
        }
    }
}

void AudioRenderer::renderWithoutAdvance (float *pOutput, uint32_t frameCount) {

    memset(pOutput, 0.f, frameCount * sizeof(float));

    renderStaticPlayCursorsWithoutAdvance(pOutput, frameCount);
    renderDynamicPlayCursorsWithoutAdvance(pOutput, frameCount);
}

void AudioRenderer::renderStaticPlayCursorsWithoutAdvance (float *pOutput, uint32_t frameCount) {

    std::lock_guard<std::mutex> sync(staticPlayCursors.playCursorsSync_);

    for (auto& [handle, playCursor]: staticPlayCursors.playCursors_) {

        double originalPos = playCursor.pos_;
        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += playCursor.getSample();
            playCursor.advance();
        }
        playCursor.pos_ = originalPos;
    }
}

void AudioRenderer::renderDynamicPlayCursorsWithoutAdvance (float *pOutput, uint32_t frameCount) {

    for (auto& playCursor: dynamicPlayCursors.playCursors_) {
        
        double originalPos = playCursor.pos_;
        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += playCursor.getSample();
            playCursor.advance();
        }
        playCursor.pos_ = originalPos;
    }
}