#pragma once


#include <atomic>
#include <cstdint>


template <typename T>
class TripleBuffer {

public:

    TripleBuffer () = default;

    TripleBuffer (const TripleBuffer&) = delete;
    TripleBuffer& operator= (const TripleBuffer&) = delete;

    T& getWriteBuffer ();
    void publish ();

    bool fetch ();
    T& getReadBuffer ();

    // Apply func to every internal buffer. Not thread-safe — call before
    // producer/consumer threads start using the buffer.
    template <typename F>
    void apply (F func);

private:

    T buffers_[3];

    uint32_t writeIdx_ = 0;
    uint32_t readIdx_  = 2;

    // bits [2:1] = middleIdx, bit [0] = dirty
    std::atomic<uint32_t> middle_{1u << 1};
};


#include "TripleBuffer.inl"
