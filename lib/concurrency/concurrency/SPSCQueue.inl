#include "SPSCQueue.hpp"


template <typename T, uint32_t Capacity>
bool SPSCQueue<T, Capacity>::push (const T& value) {

    uint32_t tail = tail_.load(std::memory_order_relaxed);
    uint32_t next = (tail + 1) % SLOTS;

    if (next == head_.load(std::memory_order_acquire))
        return false; // full

    slots_[tail] = value;
    tail_.store(next, std::memory_order_release);

    return true;
}

template <typename T, uint32_t Capacity>
bool SPSCQueue<T, Capacity>::pop (T& value) {

    uint32_t head = head_.load(std::memory_order_relaxed);

    if (head == tail_.load(std::memory_order_acquire))
        return false; // empty

    value = slots_[head];
    head_.store((head + 1) % SLOTS, std::memory_order_release);

    return true;
}
