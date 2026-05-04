#include "SlotPool.hpp"

#include <cassert>
#include <utility>


namespace fc {


template <typename T, typename Tag>
typename SlotPool<T, Tag>::Handle SlotPool<T, Tag>::insert (T value) {

    uint32_t slotIdx;
    if (freeListHead_ != NULL_IDX) {

        slotIdx = freeListHead_;
        freeListHead_ = sparse_[slotIdx].denseIdx;
    }
    else {

        slotIdx = static_cast<uint32_t>(sparse_.size());
        sparse_.emplace_back();
    }

    Slot &slot = sparse_[slotIdx];
    ++slot.generation;  // even -> odd (alive)
    slot.denseIdx = static_cast<uint32_t>(dense_.size());

    dense_.emplace_back(std::move(value));
    denseToSparse_.emplace_back(slotIdx);

    return Handle{slotIdx, slot.generation};
}


template <typename T, typename Tag>
bool SlotPool<T, Tag>::erase (Handle handle) {

    if (!contains(handle)) return false;

    Slot &slot = sparse_[handle.index];
    uint32_t denseIdx     = slot.denseIdx;
    uint32_t lastDenseIdx = static_cast<uint32_t>(dense_.size()) - 1;

    if (denseIdx != lastDenseIdx) {

        dense_[denseIdx] = std::move(dense_[lastDenseIdx]);

        uint32_t movedSlotIdx = denseToSparse_[lastDenseIdx];
        denseToSparse_[denseIdx] = movedSlotIdx;
        sparse_[movedSlotIdx].denseIdx = denseIdx;
    }

    dense_.pop_back();
    denseToSparse_.pop_back();

    ++slot.generation; // odd -> even (dead)
    slot.denseIdx = freeListHead_;
    freeListHead_ = handle.index;

    return true;
}


template <typename T, typename Tag>
T& SlotPool<T, Tag>::get (Handle handle) {

    assert(contains(handle) && "SlotPool::get called with invalid handle");
    return dense_[sparse_[handle.index].denseIdx];
}


template <typename T, typename Tag>
const T& SlotPool<T, Tag>::get (Handle handle) const {

    assert(contains(handle) && "SlotPool::get called with invalid handle");
    return dense_[sparse_[handle.index].denseIdx];
}


template <typename T, typename Tag>
T* SlotPool<T, Tag>::tryGet (Handle handle) {

    if (!contains(handle)) return nullptr;
    return &dense_[sparse_[handle.index].denseIdx];
}


template <typename T, typename Tag>
const T* SlotPool<T, Tag>::tryGet (Handle handle) const {

    if (!contains(handle)) return nullptr;
    return &dense_[sparse_[handle.index].denseIdx];
}


template <typename T, typename Tag>
bool SlotPool<T, Tag>::contains (Handle handle) const {

    return handle.index < sparse_.size() &&
           sparse_[handle.index].generation == handle.generation &&
           (handle.generation & 1u) == 1u;
}


template <typename T, typename Tag>
void SlotPool<T, Tag>::reserve (std::size_t capacity) {

    sparse_.reserve(capacity);
    dense_.reserve(capacity);
    denseToSparse_.reserve(capacity);
}


template <typename T, typename Tag>
std::size_t SlotPool<T, Tag>::size () const {

    return dense_.size();
}


template <typename T, typename Tag>
bool SlotPool<T, Tag>::empty () const {

    return dense_.empty();
}


} // namespace fc
