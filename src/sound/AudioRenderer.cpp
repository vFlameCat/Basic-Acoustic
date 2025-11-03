#include <AudioRenderer.hpp>
#include <cstring>


void AudioRenderer::renderAudio (float *pOutput, uint32_t frameCount) {

    const uint32_t overlapSize = frameCount / 2;
    
    buf_.resize(overlapSize);
    
    std::vector<float> tempBuffer(frameCount + overlapSize, 0.f);
    renderWithoutAdvance(tempBuffer.data(), frameCount + overlapSize);

    for (auto& [handle, playCursor]: staticPlayCursors.playCursors_) {

        playCursor.pos_ += playCursor.pitch * frameCount;
    }

    for (auto& playCursor: dynamicPlayCursors.playCursors_) {

        playCursor.pos_ += playCursor.pitch * frameCount;
    }
    
    for (uint32_t i = 0; i < overlapSize; ++i) {
        tempBuffer[i] += buf_[i];
        tempBuffer[i] *= 0.5;
    }
    
    memcpy(pOutput, tempBuffer.data(), frameCount * sizeof(float));
    memcpy(buf_.data(), tempBuffer.data() + frameCount, overlapSize * sizeof(float));
}

void AudioRenderer::render (float *pOutput, uint32_t frameCount) {

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

    dynamicPlayCursors.recieve();

    for (auto& playCursor: dynamicPlayCursors.playCursors_) {

        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += playCursor.getSample();
            playCursor.advance();
        }
    }
}

void AudioRenderer::renderWithoutAdvance (float *pOutput, uint32_t frameCount) {

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

    dynamicPlayCursors.recieve();

    for (auto& playCursor: dynamicPlayCursors.playCursors_) {
        
        double originalPos = playCursor.pos_;
        for (uint32_t i = 0; i < frameCount; ++i) {

            pOutput[i] += playCursor.getSample();
            playCursor.advance();
        }
        playCursor.pos_ = originalPos;
    }
}