#pragma once

#include <ferrugo/alg/math.hpp>
#include <ferrugo/alg/matrix/matrix.base.hpp>
#include <functional>
#include <numeric>

namespace ferrugo
{
namespace alg
{

namespace detail
{

struct dot_fn
{
    template <class T, class U, std::size_t D, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
    auto operator()(const vector<T, D>& lhs, const vector<U, D>& rhs) const -> Res
    {
        return std::inner_product(std::begin(lhs), std::end(lhs), std::begin(rhs), Res{});
    }
};

static constexpr inline auto dot = dot_fn{};

struct norm_fn
{
    template <class T, std::size_t D, class Res = std::invoke_result_t<std::multiplies<>, T, T>>
    auto operator()(const vector<T, D>& item) const -> Res
    {
        return dot(item, item);
    }
};

static constexpr inline auto norm = norm_fn{};

struct length_fn
{
    template <class T, std::size_t D>
    auto operator()(const vector<T, D>& item) const -> decltype(sqrt(norm(item)))
    {
        return sqrt(norm(item));
    }
};

static constexpr inline auto length = length_fn{};

struct unit_fn
{
    template <
        class T,
        std::size_t D,
        class Sqr = std::invoke_result_t<std::multiplies<>, T, T>,
        class Sqrt = decltype(sqrt(std::declval<Sqr>())),
        class Res = std::invoke_result_t<std::divides<>, T, Sqrt>>
    auto operator()(const vector<T, D>& item) const -> vector<Res, D>
    {
        const auto len = length(item);
        if (!len)
        {
            return item;
        }
        return item / len;
    }
};

static constexpr inline auto unit = unit_fn{};

struct distance_fn
{
    template <class T, class U, std::size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<U, D>& rhs) const -> decltype(length(rhs - lhs))
    {
        return length(rhs - lhs);
    }
};

static constexpr inline auto distance = distance_fn{};

struct cross_fn
{
    template <class T, class U, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
    auto operator ()(const vector_2d<T>& lhs, const vector_2d<U>& rhs) const -> Res
    {
        return lhs[0] * rhs[1] - lhs[1] * rhs[0];
    }

    template <class T, class U, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
    auto operator ()(const vector_3d<T>& lhs, const vector_3d<U>& rhs) const -> vector<Res, 3>
    {
        return vector<Res, 3>{ {
            lhs[1] * rhs[2] - lhs[2] * rhs[1],
            lhs[2] * rhs[0] - lhs[0] * rhs[2],
            lhs[0] * rhs[1] - lhs[1] * rhs[0] }};
    }
};

static constexpr inline auto cross = cross_fn{};

struct projection_fn
{
    template <class T, std::size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const -> decltype(rhs * (dot(rhs, lhs) / norm(rhs)))
    {
        return rhs * (dot(rhs, lhs) / norm(rhs));
    }
};

static constexpr inline auto projection = projection_fn{};

struct rejection_fn
{
    template <class T, std::size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const -> decltype(lhs - projection(lhs, rhs))
    {
        return lhs - projection(lhs, rhs);
    }
};

static constexpr inline auto rejection = rejection_fn{};

struct angle_fn
{
    template <class T>
    auto operator ()(const vector_2d<T>& lhs, const vector_2d<T>& rhs) const -> decltype(atan2(cross(lhs, rhs), dot(lhs, rhs)))
    {
        return atan2(cross(lhs, rhs), dot(lhs, rhs));
    }

    template <class T>
    auto operator ()(const vector_3d<T>& lhs, const vector_3d<T>& rhs) const -> decltype(acos(dot(lhs, rhs) / (length(lhs) * length(rhs))))
    {
        return acos(dot(lhs, rhs) / (length(lhs) * length(rhs)));
    }
};

static constexpr inline auto angle = angle_fn{};


}  // namespace detail

using detail::dot;
using detail::length;
using detail::norm;
using detail::unit;
using detail::distance;
using detail::cross;
using detail::projection;
using detail::rejection;
using detail::angle;

}  // namespace alg
}  // namespace ferrugo
