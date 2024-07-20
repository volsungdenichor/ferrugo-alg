#pragma once

#include <cmath>
#include <functional>

namespace ferrugo
{
namespace alg
{

namespace detail
{

struct sign_fn
{
    template <class T>
    auto operator()(T v) const -> int
    {
        static const T zero = T{};
        if (v > zero)
        {
            return +1;
        }
        else if (v < zero)
        {
            return -1;
        }
        return 0;
    }
};

struct sqr_fn
{
    template <class T>
    auto operator()(T v) const -> std::invoke_result_t<std::multiplies<>, T, T>
    {
        return v * v;
    }
};

struct sqrt_fn
{
    template <class T>
    auto operator()(T v) const -> decltype(std::sqrt(v))
    {
        return std::sqrt(v);
    }
};

struct abs_fn
{
    template <class T>
    auto operator()(T x) const -> T
    {
        return std::abs(x);
    }
};

struct floor_fn
{
    template <class T>
    auto operator()(T x) const -> decltype(std::floor(x))
    {
        return std::floor(x);
    }
};

struct ceil_fn
{
    template <class T>
    auto operator()(T x) const -> decltype(std::ceil(x))
    {
        return std::ceil(x);
    }
};

struct sin_fn
{
    template <class T>
    auto operator()(T x) const -> decltype(std::sin(x))
    {
        return std::sin(x);
    }
};

struct cos_fn
{
    template <class T>
    auto operator()(T x) const -> decltype(std::cos(x))
    {
        return std::cos(x);
    }
};

struct atan2_fn
{
    template <class T>
    auto operator()(T y, T x) const -> decltype(std::atan2(y, x))
    {
        return std::atan2(y, x);
    }
};

struct asin_fn
{
    template <class T>
    auto operator()(T x) const -> decltype(std::asin(x))
    {
        return std::asin(x);
    }
};

struct acos_fn
{
    template <class T>
    auto operator()(T x) const -> decltype(std::acos(x))
    {
        return std::acos(x);
    }
};

}  // namespace detail

static constexpr inline auto sqr = detail::sqr_fn{};
static constexpr inline auto sqrt = detail::sqrt_fn{};
static constexpr inline auto abs = detail::abs_fn{};
static constexpr inline auto floor = detail::floor_fn{};
static constexpr inline auto ceil = detail::ceil_fn{};
static constexpr inline auto sin = detail::sin_fn{};
static constexpr inline auto cos = detail::cos_fn{};
static constexpr inline auto atans = detail::atan2_fn{};
static constexpr inline auto asin = detail::asin_fn{};
static constexpr inline auto acos = detail::acos_fn{};
static constexpr inline auto sign = detail::sign_fn{};

}  // namespace alg
}  // namespace ferrugo
