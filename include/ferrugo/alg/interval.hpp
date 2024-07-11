#pragma once

#include <functional>
#include <iostream>

namespace ferrugo
{
namespace alg
{
template <class T>
struct interval : std::array<T, 2>
{
    using base_t = std::array<T, 2>;

    interval(T lo, T up) : base_t{ lo, up }
    {
    }

    interval() : interval(T{}, T{})
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const interval& item)
    {
        return os << "[" << item[0] << ", " << item[1] << ")";
    }
};

template <class T, class U>
bool operator==(const interval<T>& lhs, const interval<U>& rhs)
{
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
}

template <class T, class U>
bool operator!=(const interval<T>& lhs, const interval<U>& rhs)
{
    return !(lhs == rhs);
}

template <class T, class U, class = std::invoke_result_t<std::plus<>, T, U>>
auto operator+=(interval<T>& lhs, U rhs) -> interval<T>&
{
    std::transform(std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::plus<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class T, class U, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator+(const interval<T>& lhs, U rhs) -> interval<Res>
{
    interval<Res> result{};
    std::transform(std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::plus<>{}, std::placeholders::_1, rhs));
    return result;
}

template <class T, class U, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator+(T lhs, const interval<U>& rhs) -> interval<Res>
{
    return rhs + lhs;
}

template <class T, class U, class = std::invoke_result_t<std::minus<>, T, U>>
auto operator-=(interval<T>& lhs, U rhs) -> interval<T>&
{
    std::transform(std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::minus<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class T, class U, class Res = std::invoke_result_t<std::minus<>, T, U>>
auto operator-(const interval<T>& lhs, U rhs) -> interval<Res>
{
    interval<Res> result{};
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::minus<>{}, std::placeholders::_1, rhs));
    return result;
}

template <class T, class U, class = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*=(interval<T>& lhs, U rhs) -> interval<T>&
{
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::multiplies<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class T, class U, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(const interval<T>& lhs, U rhs) -> interval<Res>
{
    interval<Res> result{};
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::multiplies<>{}, std::placeholders::_1, rhs));
    return result;
}

template <class T, class U, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(T lhs, const interval<U>& rhs) -> interval<Res>
{
    return rhs * lhs;
}

template <class T, class U, class = std::invoke_result_t<std::divides<>, T, U>>
auto operator/=(interval<T>& lhs, U rhs) -> interval<T>&
{
    std::transform(std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::divides<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class T, class U, class Res = std::invoke_result_t<std::divides<>, T, U>>
auto operator/(const interval<T>& lhs, U rhs) -> interval<Res>
{
    interval<Res> result{};
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::divides<>{}, std::placeholders::_1, rhs));
    return result;
}

}  // namespace alg
}  // namespace ferrugo
