#pragma once

#include <cmath>

namespace ferrugo
{
namespace mat
{

namespace detail
{

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

}  // namespace detail

static constexpr inline auto sqrt = detail::sqrt_fn{};
static constexpr inline auto abs = detail::abs_fn{};
static constexpr inline auto floor = detail::floor_fn{};
static constexpr inline auto ceil = detail::ceil_fn{};
static constexpr inline auto sin = detail::sin_fn{};
static constexpr inline auto cos = detail::cos_fn{};

}  // namespace mat
}  // namespace ferrugo