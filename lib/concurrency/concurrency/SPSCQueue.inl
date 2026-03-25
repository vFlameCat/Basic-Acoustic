#include "SPSCQueue.hpp"


template <typename T, uint32_t Capacity>
bool SPSCQueue<T, Capacity>::push (const T& value) {

    uint32_t tail = tail_.load();
    uint32_t next = (tail + 1) % SLOTS;

    if (next == head_.load())
        return false; // full

    slots_[tail] = value;
    tail_.store(next);

    return true;
}

template <typename T, uint32_t Capacity>
bool SPSCQueue<T, Capacity>::pop (T& value) {

    uint32_t head = head_.load();

    if (head == tail_.load())
        return false; // empty

    value = slots_[head];
    head_.store((head + 1) % SLOTS);

    return true;
}
