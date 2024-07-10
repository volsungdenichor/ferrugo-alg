#pragma once

#include <functional>

namespace ferrugo
{
namespace alg
{
template <class T>
struct interval
{
    T lower;
    T upper;

    interval(T lo, T up) : lower(lo), upper(up)
    {
    }

    interval() : interval(T{}, T{})
    {
    }

    bool empty() const
    {
        return lower == upper;
    }

    T size() const
    {
        return upper - lower;
    }
};

template <class T, class U>
bool operator==(const interval<T>& lhs, const interval<U>& rhs)
{
    return lhs.lower == rhs.lower && lhs.upper == rhs.upper;
}

template <class T, class U>
bool operator!=(const interval<T>& lhs, const interval<U>& rhs)
{
    return !(lhs == rhs);
}

template <class T, class U, class = std::invoke_result_t<std::plus<>, T, U>>
auto operator+=(interval<T>& lhs, U rhs) -> interval<T>&
{
    lhs.lower += rhs;
    lhs.upper += rhs;
    return lhs;
}

template <class T, class U, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator+(const interval<T>& lhs, U rhs) -> interval<Res>
{
    return interval<Res>{ static_cast<Res>(lhs.lower + rhs), static_cast<Res>(lhs.upper + rhs) };
}

template <class T, class U, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator+(T lhs, const interval<U>& rhs) -> interval<Res>
{
    return rhs + lhs;
}

template <class T, class U, class = std::invoke_result_t<std::minus<>, T, U>>
auto operator-=(interval<T>& lhs, U rhs) -> interval<T>&
{
    lhs.lower -= rhs;
    lhs.upper -= rhs;
    return lhs;
}

template <class T, class U, class Res = std::invoke_result_t<std::minus<>, T, U>>
auto operator-(const interval<T>& lhs, U rhs) -> interval<Res>
{
    return interval<Res>{ static_cast<Res>(lhs.lower - rhs), static_cast<Res>(lhs.upper - rhs) };
}

template <class T, class U, class = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*=(interval<T>& lhs, U rhs) -> interval<T>&
{
    lhs.lower *= rhs;
    lhs.upper *= rhs;
    return lhs;
}

template <class T, class U, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(const interval<T>& lhs, U rhs) -> interval<Res>
{
    return interval<Res>{ static_cast<Res>(lhs.lower * rhs), static_cast<Res>(lhs.upper * rhs) };
}

template <class T, class U, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(T lhs, const interval<U>& rhs) -> interval<Res>
{
    return rhs * lhs;
}

template <class T, class U, class = std::invoke_result_t<std::divides<>, T, U>>
auto operator/=(interval<T>& lhs, U rhs) -> interval<T>&
{
    lhs.lower /= rhs;
    lhs.upper /= rhs;
    return lhs;
}

template <class T, class U, class Res = std::invoke_result_t<std::divides<>, T, U>>
auto operator/(const interval<T>& lhs, U rhs) -> interval<Res>
{
    return interval<Res>{ static_cast<Res>(lhs.lower / rhs), static_cast<Res>(lhs.upper / rhs) };
}

}  // namespace alg
}  // namespace ferrugo
