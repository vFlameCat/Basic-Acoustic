#pragma once


#include <atomic>
#include <array>


template <typename T, uint32_t Capacity>
class SPSCQueue {

public:

    SPSCQueue () = default;

    SPSCQueue (const SPSCQueue&) = delete;
    SPSCQueue& operator= (const SPSCQueue&) = delete;

    bool push (const T& value);
    bool pop (T& value);

private:

    static constexpr uint32_t SLOTS = Capacity + 1;

    std::array<T, SLOTS> slots_{};

    std::atomic<uint32_t> head_{0};
    std::atomic<uint32_t> tail_{0};
};


#include "SPSCQueue.inl"
