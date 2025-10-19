#pragma once


#include <raylib.h>


namespace fc {


template <typename T>
class Vector3 final {

public:

    constexpr Vector3 () = default;
    constexpr Vector3 (T X, T Y, T Z);

    template <typename U>
    constexpr explicit operator Vector3<U> () const;

    constexpr T length () const;
    constexpr T lengthSquared () const;

    constexpr Vector3 normalize () const;

    constexpr T dot (const Vector3 &rhs) const;
    constexpr Vector3 cross (const Vector3 &rhs) const;

    constexpr Vector3 componentWiseMul (const Vector3 &rhs) const;
    constexpr Vector3 componentWiseDiv (const Vector3 &rhs) const;

    // compatibility with raylib methods
    constexpr Vector3 (::Vector3 vec);
    constexpr operator ::Vector3 () const;

public:

    T x{}, y{}, z{};
};  


template <typename T>
constexpr Vector3<T>& operator+= (Vector3<T> &left, const Vector3<T> &right);

template <typename T>
constexpr Vector3<T> operator+ (const Vector3<T> &left, const Vector3<T> &right);

template <typename T>
constexpr Vector3<T>& operator-= (Vector3<T> &left, const Vector3<T> &right);

template <typename T>
constexpr Vector3<T> operator- (const Vector3<T> &left, const Vector3<T> &right);

template <typename T>
constexpr Vector3<T> operator- (const Vector3<T> &left);


template <typename T>
constexpr Vector3<T>& operator*= (Vector3<T> &left, T right);

template <typename T>
constexpr Vector3<T> operator* (const Vector3<T> &left, T right);

template <typename T>
constexpr Vector3<T> operator* (T left, const Vector3<T> &right);

template <typename T>
constexpr Vector3<T>& operator/= (Vector3<T> &left, T right);

template <typename T>
constexpr Vector3<T> operator/ (const Vector3<T> &left, T right);


template <typename T>
constexpr bool operator== (const Vector3<T> &left, const Vector3<T> &right);


template <typename T>
constexpr T distance (const Vector3<T> &left, const Vector3<T> &right);

template <typename T>
constexpr T distanceSquared (const Vector3<T> &left, const Vector3<T> &right);


using Vector3f = Vector3<float>;


} // namespace fc


#include "Vector3.inl"
