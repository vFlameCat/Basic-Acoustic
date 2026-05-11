#pragma once


#include <Players/Player.hpp>
#include <Players/PlayersPool.hpp>
#include <Players/SpatialFramePlayers.hpp>
#include <AudioStorage.hpp>
#include <AudioRenderer.hpp>

#include <miniaudio.hpp>

#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>


class AudioEngine final {

public:

    AudioEngine ();

    AudioEngine (const AudioEngine&) = delete;
    AudioEngine& operator= (const AudioEngine&) = delete;
    AudioEngine (AudioEngine&&) = delete;
    AudioEngine& operator= (AudioEngine&&) = delete;

    ~AudioEngine ();

    static void callback (ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    void start ();
    void stop ();

    PlayersPool&         getPlayersPool ();
    SpatialFramePlayers& getSpatialFramePlayers ();

    std::chrono::microseconds getLastCallbackDuration () const {

        return lastCallbackDuration_.load(std::memory_order_relaxed);
    }

    std::chrono::microseconds getAvgCallbackDuration () const {

        return avgCallbackDuration_.load(std::memory_order_relaxed);
    }

private:

    void initDevice ();

private:

    static constexpr std::size_t kCallbackHistorySize = 64;

    // microseconds is an 8-byte trivially-copyable duration, so std::atomic<microseconds>
    // is lock-free on the platforms we care about. Static-assert it so an exotic toolchain
    // can't silently fall back to a mutex.
    static_assert(std::atomic<std::chrono::microseconds>::is_always_lock_free);

    ma_device device_{};

    AudioRenderer renderer_{};

    // Audio thread only — ring buffer + running sum for the rolling average.
    std::array<std::chrono::microseconds, kCallbackHistorySize> callbackHistory_{};
    std::size_t               callbackHistoryIdx_ = 0;
    std::chrono::microseconds callbackHistorySum_{0};

    // Published to UI thread.
    std::atomic<std::chrono::microseconds> lastCallbackDuration_{std::chrono::microseconds{0}};
    std::atomic<std::chrono::microseconds> avgCallbackDuration_ {std::chrono::microseconds{0}};
};
