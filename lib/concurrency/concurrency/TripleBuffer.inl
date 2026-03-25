#include "TripleBuffer.hpp"


template <typename T>
T& TripleBuffer<T>::getWriteBuffer () {

    return buffers_[writeIdx_];
}

template <typename T>
void TripleBuffer<T>::publish () {

    uint32_t old = middle_.exchange((writeIdx_ << 1) | 1u);
    writeIdx_ = old >> 1;
}

template <typename T>
bool TripleBuffer<T>::fetch () {

    uint32_t val = middle_.load();
    if (!(val & 1u)) return false;

    uint32_t old = middle_.exchange(readIdx_ << 1);
    readIdx_ = old >> 1;

    return true;
}

template <typename T>
T& TripleBuffer<T>::getReadBuffer () {

    return buffers_[readIdx_];
}
