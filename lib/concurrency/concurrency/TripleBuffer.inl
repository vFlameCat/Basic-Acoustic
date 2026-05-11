#include "TripleBuffer.hpp"


template <typename T>
T& TripleBuffer<T>::getWriteBuffer () {

    return buffers_[writeIdx_];
}

template <typename T>
void TripleBuffer<T>::publish () {

    uint32_t old = middle_.exchange((writeIdx_ << 1) | 1u, std::memory_order_acq_rel);
    writeIdx_ = old >> 1;
}

template <typename T>
bool TripleBuffer<T>::fetch () {

    uint32_t val = middle_.load(std::memory_order_acquire);
    if (!(val & 1u)) return false;

    uint32_t old = middle_.exchange(readIdx_ << 1, std::memory_order_acq_rel);
    readIdx_ = old >> 1;

    return true;
}

template <typename T>
T& TripleBuffer<T>::getReadBuffer () {

    return buffers_[readIdx_];
}

template <typename T>
template <typename F>
void TripleBuffer<T>::apply (F func) {

    for (auto &buf : buffers_) {

        func(buf);
    }
}
