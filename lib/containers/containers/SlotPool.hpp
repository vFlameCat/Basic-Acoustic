#pragma once


#include <vector>
#include <cstdint>
#include <cstddef>


namespace fc {


// Tag-scoped generational handle. Handles with different Tag are distinct
// types — the compiler refuses to mix them.
template <typename Tag>
struct SlotHandle {

    uint32_t index      = 0;
    uint32_t generation = 0;

    bool isValid () const { return (generation & 1u) == 1u; }

    bool operator== (const SlotHandle&) const = default;

    static const SlotHandle Invalid;
};


template <typename Tag>
inline const SlotHandle<Tag> SlotHandle<Tag>::Invalid{};


// Dense slot pool with stable, generational handles.
//
// Storage layout:
//   sparse_[index]  -> Slot{ denseIdx, generation }
//   dense_          -> packed values (no holes), iterated cache-friendly
//   denseToSparse_  -> reverse map for swap-and-pop on erase
//
// Generation rules per slot:
//   0       = never used
//   odd     = alive (handle valid)
//   even>0  = dead (slot lives in free-list)
//
// The Tag template parameter scopes Handle's identity:
//   pools that share the same Tag share the Handle type and so can produce
//   interchangeable handles (useful for cross-thread mirrors with different
//   stored types). Pools with different Tag have incompatible Handle types,
//   so handles from unrelated pools cannot be mixed by accident.
template <typename T, typename Tag = T>
class SlotPool final {

public:

    using Handle = SlotHandle<Tag>;


    SlotPool () = default;

    SlotPool (const SlotPool&) = delete;
    SlotPool& operator= (const SlotPool&) = delete;
    SlotPool (SlotPool&&) noexcept = default;
    SlotPool& operator= (SlotPool&&) noexcept = default;


    Handle insert (T value);
    bool   erase  (Handle handle);

    T&       get      (Handle handle);             // requires contains(handle)
    const T& get      (Handle handle) const;       // requires contains(handle)

    T*       tryGet   (Handle handle);             // nullptr if !contains(handle)
    const T* tryGet   (Handle handle) const;       // nullptr if !contains(handle)

    bool     contains (Handle handle) const;

    void reserve (std::size_t capacity);

    std::size_t size  () const;
    bool        empty () const;

    auto begin  ()       { return dense_.begin();  }
    auto end    ()       { return dense_.end();    }
    auto begin  () const { return dense_.begin();  }
    auto end    () const { return dense_.end();    }
    auto cbegin () const { return dense_.cbegin(); }
    auto cend   () const { return dense_.cend();   }

private:

    struct Slot {

        // alive: index in dense_
        // dead:  next free-list link (or NULL_IDX)
        uint32_t denseIdx   = 0;
        uint32_t generation = 0;
    };

    static constexpr uint32_t NULL_IDX = static_cast<uint32_t>(-1);

    std::vector<Slot>     sparse_{};
    std::vector<T>        dense_{};
    std::vector<uint32_t> denseToSparse_{};

    uint32_t freeListHead_ = NULL_IDX;
};


} // namespace fc


#include "SlotPool.inl"
