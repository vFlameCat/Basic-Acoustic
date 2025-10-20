#include <Vector3.hpp>

#include <cmath>
#include <cassert>


namespace fc {
    

template <typename T>
constexpr Vector3<T>::Vector3 (T X, T Y, T Z): x(X), y(Y), z(Z) {}


template <typename T>
template <typename U>
constexpr Vector3<T>::operator Vector3<U> () const {

    return Vector3<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
}

template <typename T>
constexpr T Vector3<T>::length () const {

    static_assert(std::is_floating_point_v<T>, "Vector3::length() is only supported for floating point types");

    return std::sqrt(lengthSquared());
}

template <typename T>
constexpr T Vector3<T>::lengthSquared () const {

    return dot(*this);
}


template <typename T>
constexpr Vector3<T> Vector3<T>::normalize () const {

    static_assert(std::is_floating_point_v<T>, "Vector3::normalized() is only supported for floating point types");
    assert(*this != Vector3<T>() && "Vector3::normalized() cannot normalize a zero vector");

    return *this / length();
}


template <typename T>
constexpr T Vector3<T>::dot (const Vector3 &rhs) const {

    return x * rhs.x + y * rhs.y + z * rhs.z;
}

template <typename T>
constexpr Vector3<T> Vector3<T>::cross (const Vector3 &rhs) const {

    return Vector3<T>((y * rhs.z) - (z * rhs.y), (z * rhs.x) - (x * rhs.z), (x * rhs.y) - (y * rhs.x));
}


template <typename T>
constexpr Vector3<T> Vector3<T>::componentWiseMul (const Vector3 &rhs) const {

    return Vector3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
}

template <typename T>
constexpr Vector3<T> Vector3<T>::componentWiseDiv (const Vector3 &rhs) const {

    assert(rhs.x != 0 && "Vector3::componentWiseDiv() cannot divide by 0");
    assert(rhs.y != 0 && "Vector3::componentWiseDiv() cannot divide by 0");
    assert(rhs.z != 0 && "Vector3::componentWiseDiv() cannot divide by 0");

    return Vector3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
}

// compatibility with raylib methods start

template <>
constexpr Vector3f::Vector3 (::Vector3 vec): x(vec.x), y(vec.y), z(vec.z) {}

template <>
constexpr Vector3f::operator ::Vector3 () const {

    return ::Vector3(x, y, z);
}

// compatibility with raylib methods end


template <typename T>
constexpr Vector3<T>& operator+= (Vector3<T> &left, const Vector3<T> &right) {

    left.x += right.x;
    left.y += right.y;
    left.z += right.z;

    return left;
}

template <typename T>
constexpr Vector3<T> operator+ (const Vector3<T> &left, const Vector3<T> &right) {

    Vector3<T> tmp = left;
    return tmp += right;
}

template <typename T>
constexpr Vector3<T>& operator-= (Vector3<T> &left, const Vector3<T> &right) {

    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;

    return left;
}

template <typename T>
constexpr Vector3<T> operator- (const Vector3<T> &left, const Vector3<T> &right) {

    Vector3<T> tmp = left;
    return tmp -= right;
}

template <typename T>
constexpr Vector3<T> operator- (const Vector3<T> &left) {

    return Vector3<T>(-left.x, -left.y, -left.z);
}


template <typename T>
constexpr Vector3<T>& operator*= (Vector3<T> &left, T right) {

    left.x *= right;
    left.y *= right;
    left.z *= right;

    return left;
}

template <typename T>
constexpr Vector3<T> operator* (const Vector3<T> &left, T right) {

    Vector3<T> tmp = left;
    return tmp *= right;
}

template <typename T>
constexpr Vector3<T> operator* (T left, const Vector3<T> &right) {

    Vector3<T> tmp = right;
    return tmp *= left;
}

template <typename T>
constexpr Vector3<T>& operator/= (Vector3<T> &left, T right) {

    assert(right != 0 && "Vector3::operator/= cannot divide by 0");

    left.x /= right;
    left.y /= right;
    left.z /= right;

    return left;
}

template <typename T>
constexpr Vector3<T> operator/ (const Vector3<T> &left, T right) {

    assert(right != 0 && "Vector3::operator/ cannot divide by 0");

    Vector3<T> tmp = left;
    return tmp /= right;
}


template <typename T>
constexpr bool operator== (const Vector3<T> &left, const Vector3<T> &right) {

    return left.x == right.x && left.y == right.y && left.z == right.z; 
}


template <typename T>
constexpr T distance (const Vector3<T> &left, const Vector3<T> &right) {

    static_assert(std::is_floating_point_v<T>, "Vector3::distance() is only supported for floating point types");

    return std::sqrt(distanceSquared(left, right));
}

template <typename T>
constexpr T distanceSquared (const Vector3<T> &left, const Vector3<T> &right) {

    return (right - left).lengthSquared();
}


} // namespace fc
