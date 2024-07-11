#pragma once

#include <ferrugo/alg/interval.hpp>
#include <ferrugo/alg/linear.hpp>
#include <ferrugo/alg/polygon.hpp>
#include <ferrugo/alg/circular.hpp>

#if 0

namespace ferrugo
{
namespace alg
{
namespace detail
{

struct lower_fn
{
    template <class T>
    auto operator()(const interval<T>& item) const -> T
    {
        return item.lower;
    }
};

static constexpr inline auto lower = lower_fn{};

struct upper_fn
{
    template <class T>
    auto operator()(const interval<T>& item) const -> T
    {
        return item.upper;
    }
};

static constexpr inline auto upper = upper_fn{};

struct center_fn
{
};

static constexpr inline auto center = center_fn{};

struct size_fn
{
    template <class T>
    auto operator()(const interval<T>& item) const -> T
    {
        return item.upper - item.lower;
    }
};

static constexpr inline auto size = size_fn{};

struct contains_fn
{
    template <class T, class U>
    auto operator()(const interval<T>& item, U value) const -> bool
    {
        return item.lower <= value && value < item.upper;
    }
};

static constexpr inline auto contains = contains_fn{};

struct intersects_fn
{
    template <class T, class U>
    auto operator()(const interval<T>& self, const interval<U>& other) const -> bool
    {
        return false;
    }
};

static constexpr inline auto intersects = intersects_fn{};

struct intersection_fn
{
    template <class T, size_t D, class Tag1, class Tag2, class E = T>
    auto operator ()(
        const linear_shape<T, D, Tag1>& lhs,
        const linear_shape<T, D, Tag2>& rhs,
        E epsilon = {}) const -> core::optional<vector<T, D>>
    {
        using lhs_traits = linear_shape_traits<linear_shape<T, D, Tag1>>;
        using rhs_traits = linear_shape_traits<linear_shape<T, D, Tag2>>;

        auto par = get_line_intersection_parameters(lhs[0], lhs[1], rhs[0], rhs[1], epsilon);

        return par && lhs_traits::contains_parameter(std::get<0>(*par)) && rhs_traits::contains_parameter(std::get<1>(*par))
            ? interpolate(lhs[0], lhs[1], std::get<0>(*par))
            : core::optional<vector<T, D>>{ core::none };
    }
};

static constexpr inline auto intersection = intersection_fn{};

struct altitude_fn
{
    template <typename T>
    auto operator ()(const triangle_2d<T>& value, size_t index) const -> segment_2d<T>
    {
        static const T epsilon = T(0.1);

        auto v = value[(index + 0) % 3];

        auto p = projection(v, make_line(value[(index + 1) % 3], value[(index + 2) % 3]), epsilon);

        return{ v, *p };
    }
};

static constexpr inline auto altitude = altitude_fn{};

struct centroid_fn
{
    template <typename T>
    auto operator ()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        return core::accumulate(value._data, vector_2d<T>{}) / 3;
    }
};

static constexpr inline auto centroid = centroid_fn{};

struct orthocenter_fn
{
    template <typename T>
    auto operator ()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        static const T epsilon = T(0.0001);
        static constexpr auto _altitude = altitude_fn{};

        return *intersection(make_line(_altitude(value, 0)), make_line(_altitude(value, 1)), epsilon);
    }
};

static constexpr inline auto orthocenter = orthocenter_fn{};

struct circumcenter_fn
{
    template <typename T>
    auto operator ()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        static T epsilon = T(0.0001);

        auto s0 = get_segment(value, 0);
        auto s1 = get_segment(value, 1);

        return *intersection(make_line(perpendicular(s0, center(s0))), make_line(perpendicular(s1, center(s1))), epsilon);
    }
};

static constexpr inline auto circumcenter = circumcenter_fn{};

struct incenter_fn
{
    template <typename T>
    auto operator ()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        T sum = T(0);

        vector_2d<T> result;

        for (size_t i = 0; i < 3; ++i)
        {
            auto side_length = length(get_segment(value, i));

            result += side_length * value[(i + 2) % 3];

            sum += side_length;
        }

        return result / sum;
    }
};

static constexpr inline auto incenter = incenter_fn{};

struct incircle_fn
{
    template <class T>
    auto operator ()(const triangle_2d<T>& triangle) const -> circle_2d<T>
    {
        static const T epsilon = T(0.1);

        auto center = incenter(triangle);
        auto radius = distance(center, *projection(center, get_segment(triangle, 0), epsilon));

        return circle<T> { center, radius };
    }
};

static constexpr inline auto incircle = incircle_fn{};

struct circumcircle_fn
{
    template <class T>
    auto operator ()(const triangle_2d<T>& triangle) const -> circle_2d<T>
    {
        auto center = circumcenter(triangle);
        auto radius = distance(center, get_vertex(triangle, 0));

        return circle<T> { center, radius };
    }
};

static constexpr inline auto circumcircle = circumcircle_fn{};

struct projection_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const
    {
        return math::projection(lhs, rhs);
    }

    template <class T, size_t D, class Tag, class E = T>
    auto operator ()(
        const vector<T, D>& point,
        const linear_shape<T, D, Tag>& shape,
        E epsilon = {}) const -> core::optional<vector<T, D>>
    {
        using traits = linear_shape_traits<linear_shape<T, D, Tag>>;

        auto p0 = shape[0];
        auto p1 = shape[1];

        auto result = p0 + (*this)(point - p0, p1 - p0);

        auto t = get_line_intersection_parameter(p0, p1, result, epsilon);

        return core::make_optional(t && traits::contains_parameter(*t), result);
    }
};

struct normal_fn
{
    template <class T, class Tag>
    auto operator ()(const linear_shape<T, 2, Tag>& value) const -> vector<T, 2>
    {
        return perpendicular(direction(value));
    }
};


static constexpr inline auto normal = normal_fn{};

struct perpendicular_fn
{
    template <class T>
    auto operator ()(const vector<T, 2>& value) const -> vector<T, 2>
    {
        return math::perpendicular(value);
    }

    template <class T, class Tag>
    auto operator ()(const linear_shape<T, 2, Tag>& value, const vector<T, 2>& origin) const -> linear_shape<T, 2, Tag>
    {
        return{ origin, origin + (*this)(value[1] - value[0]) };
    }

    template <class T, class Tag>
    auto operator ()(const linear_shape<T, 2, Tag>& value) const -> linear_shape<T, 2, Tag>
    {
        return (*this)(value, value[0]);
    }
};

static constexpr inline auto perpendicular = perpendicular_fn{};


}  // namespace detail

using detail::center;
using detail::contains;
using detail::intersects;
using detail::lower;
using detail::size;
using detail::upper;
using detail::altitude;
using detail::centroid;
using detail::orthocenter;
using detail::intersection;
using detail::circumcenter;
using detail::incenter;
using detail::incircle;
using detail::circumcircle;
using detail::projection;
using detail::normal;
using detail::perpendicular;

}  // namespace alg
}  // namespace ferrugo

#endif