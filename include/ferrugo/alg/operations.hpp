#pragma once

#include <ferrugo/alg/circular.hpp>
#include <ferrugo/alg/interval.hpp>
#include <ferrugo/alg/linear.hpp>
#include <ferrugo/alg/math.hpp>
#include <ferrugo/alg/polygon.hpp>
#include <ferrugo/alg/region.hpp>
#include <numeric>
#include <optional>

namespace ferrugo
{
namespace alg
{
namespace detail
{

template <class T>
bool between(T v, T lo, T up)
{
    return lo <= v && v < up;
}

template <class T>
bool inclusive_between(T v, T lo, T up)
{
    return lo <= v && v <= up;
}

template <class T, class E>
auto approx_equal(T value, E epsilon)
{
    return [=](auto v) { return std::abs(v - value) < epsilon; };
}

template <class T, std::size_t D>
line<T, 2> make_line(const segment<T, D>& s)
{
    return line<T, 2>{ s[0], s[1] };
}

struct dot_fn
{
    template <class T, class U, std::size_t D, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
    auto operator()(const vector<T, D>& lhs, const vector<U, D>& rhs) const -> Res
    {
        return std::inner_product(std::begin(lhs), std::end(lhs), std::begin(rhs), Res{});
    }
};

static constexpr inline auto dot = dot_fn{};

struct cross_fn
{
    template <class T, class U, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
    auto operator()(const vector_2d<T>& lhs, const vector_2d<U>& rhs) const -> Res
    {
        return lhs[0] * rhs[1] - lhs[1] * rhs[0];
    }

    template <class T, class U, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
    auto operator()(const vector_3d<T>& lhs, const vector_3d<U>& rhs) const -> vector<Res, 3>
    {
        return vector<Res, 3>{
            { lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2], lhs[0] * rhs[1] - lhs[1] * rhs[0] }
        };
    }
};

static constexpr inline auto cross = cross_fn{};

struct angle_fn
{
    template <class T>
    auto operator()(const vector_2d<T>& lhs, const vector_2d<T>& rhs) const
        -> decltype(atan2(cross(lhs, rhs), dot(lhs, rhs)))
    {
        return atan2(cross(lhs, rhs), dot(lhs, rhs));
    }

    template <class T>
    auto operator()(const vector_3d<T>& lhs, const vector_3d<T>& rhs) const
        -> decltype(acos(dot(lhs, rhs) / (length(lhs) * length(rhs))))
    {
        return acos(dot(lhs, rhs) / (length(lhs) * length(rhs)));
    }
};

static constexpr inline auto angle = angle_fn{};

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

    template <class T, std::size_t D>
    auto operator()(const segment<T, D>& item) const
    {
        return (*this)(item[1] - item[0]);
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
    auto operator()(const vector<T, D>& lhs, const vector<U, D>& rhs) const -> decltype(length(rhs - lhs))
    {
        return length(rhs - lhs);
    }
};

static constexpr inline auto distance = distance_fn{};

template <std::size_t Dim>
struct lower_upper_fn
{
    template <class T>
    auto operator()(const interval<T>& item) const -> T
    {
        return item[Dim];
    }

    template <class T, std::size_t D>
    auto operator()(const region<T, D>& item) const -> vector<T, D>
    {
        vector<T, D> result{ raw };
        for (std::size_t d = 0; d < D; ++d)
        {
            result[d] = (*this)(item[d]);
        }
        return result;
    }
};

static constexpr inline auto lower = lower_upper_fn<0>{};
static constexpr inline auto upper = lower_upper_fn<1>{};

struct size_fn
{
    template <class T>
    auto operator()(const interval<T>& item) const -> T
    {
        return upper(item) - lower(item);
    }

    template <class T, std::size_t D>
    auto operator()(const region<T, D>& item) const -> vector<T, D>
    {
        return upper(item) - lower(item);
    }
};

static constexpr inline auto size = size_fn{};

struct center_fn
{
    template <class T, std::size_t D>
    auto operator()(const region<T, D>& item) const -> vector<T, D>
    {
        return (lower(item) + upper(item)) / 2;
    }

    template <class T, std::size_t D>
    auto operator()(const segment<T, D>& item) const -> vector<T, D>
    {
        return (item[0] + item[1]) / 2;
    }

    template <class T, std::size_t D>
    auto operator()(const circular_shape<T, D>& item) const -> vector<T, D>
    {
        return item.center;
    }
};

static constexpr inline auto center = center_fn{};

struct orientation_fn
{
    template <class T, class U>
    auto operator()(const vector_2d<T>& point, const vector_2d<U>& start, const vector_2d<U>& end) const
    {
        return cross(end - start, point - start);
    }

    template <class T, class U, class Tag>
    auto operator()(const vector_2d<T>& point, const linear_shape<Tag, U, 2>& shape) const
    {
        return (*this)(point, shape[0], shape[1]);
    }
};

static constexpr inline auto orientation = orientation_fn{};

struct contains_fn
{
    template <class T, class U>
    auto operator()(const interval<T>& item, U value) const -> bool
    {
        return between(value, lower(item), upper(item));
    }

    template <class T>
    auto operator()(const interval<T>& item, const interval<T>& other) const -> bool
    {
        const T lo = lower(item);
        const T up = upper(item);
        return inclusive_between(other[0], lo, up) && inclusive_between(other[1], lo, up);
    }

    template <class T, std::size_t D>
    auto operator()(const region<T, D>& item, const region<T, D>& other) const -> bool
    {
        for (std::size_t d = 0; d < D; ++d)
        {
            if (!(*this)(item[d], other[d]))
            {
                return false;
            }
        }
        return true;
    }

    template <class T, class U, std::size_t D>
    auto operator()(const circular_shape<T, D>& item, const vector<U, D>& other) const -> bool
    {
        return norm(other - center(item)) <= sqr(item.radius);
    }

    template <class T, class U>
    bool operator()(const triangle<T, 2>& item, const vector<U, 2>& other) const
    {
        static const auto same_sign = [](int a, int b) { return (a <= 0 && b <= 0) || (a >= 0 && b >= 0); };

        int result[3];

        for (std::size_t i = 0; i < 3; ++i)
        {
            result[i] = sign(orientation(other, segment<T, 2>{ item[(i + 0) % 3], item[(i + 1) % 3] }));
        }

        return same_sign(result[0], result[1]) && same_sign(result[0], result[2]) && same_sign(result[1], result[2]);
    }
};

static constexpr inline auto contains = contains_fn{};

struct intersects_fn
{
    template <class T>
    auto operator()(const interval<T>& self, const interval<T>& other) const -> bool
    {
        return inclusive_between(self[0], lower(other), upper(other))     //
               || inclusive_between(self[1], lower(other), upper(other))  //
               || inclusive_between(other[0], lower(self), upper(self))   //
               || inclusive_between(other[1], lower(self), upper(self));
    }

    template <class T, std::size_t D>
    auto operator()(const region<T, D>& self, const region<T, D>& other) const -> bool
    {
        for (std::size_t d = 0; d < D; ++d)
        {
            if (!(*this)(self[d], other[d]))
            {
                return false;
            }
        }
        return true;
    }
};

static constexpr inline auto intersects = intersects_fn{};

struct interpolate_fn
{
    template <class R, class T, std::size_t D>
    auto operator()(R r, const vector<T, D>& lhs, const vector<T, D>& rhs) const -> vector<T, D>
    {
        return (lhs * r) + (rhs * (R(1) - r));
    }

    template <class R, class T, std::size_t D>
    auto operator()(R r, const segment<T, D>& value) const
    {
        return (*this)(r, value[0], value[1]);
    }

    template <class R, class T>
    auto operator()(R r, const interval<T>& item) const
    {
        return lower(item) * r + upper(item) * (R(1) - r);
    }
};

static constexpr inline auto interpolate = interpolate_fn{};

template <class T, class E>
auto get_line_intersection_parameter(const vector<T, 2>& a0, const vector<T, 2>& a1, const vector<T, 2>& p, E epsilon)
    -> std::optional<T>
{
    const auto dir = a1 - a0;

    const auto d = p - a0;

    const auto det = cross(dir, d);
    if (approx_equal(E(0), epsilon)(det))
    {
        return {};
    }

    return dot(d, dir) / norm(dir);
}

template <class T, class E>
auto get_line_intersection_parameters(
    const vector<T, 2>& a0, const vector<T, 2>& a1, const vector<T, 2>& b0, const vector<T, 2>& b1, E epsilon)
    -> std::optional<std::tuple<T, T>>
{
    const auto dir_a = a1 - a0;
    const auto dir_b = b1 - b0;

    const auto det = cross(dir_a, dir_b);
    const auto v = b0 - a0;

    if (approx_equal(E(0), epsilon)(det))
    {
        return {};
    }

    return { { cross(v, dir_b) / det, cross(v, dir_a) / det } };
}

template <class T>
bool contains_param(line_tag, T)
{
    return true;
}

template <class T>
bool contains_param(ray_tag, T v)
{
    return v >= T(0);
}

template <class T>
bool contains_param(segment_tag, T v)
{
    return T(0) <= v && v <= T(1);
}

struct intersection_fn
{
    template <class T, std::size_t D, class Tag1, class Tag2, class E = T>
    auto operator()(const linear_shape<Tag1, T, D>& lhs, const linear_shape<Tag2, T, D>& rhs, E epsilon = {}) const
        -> std::optional<vector<T, D>>
    {
        const auto par = get_line_intersection_parameters(lhs[0], lhs[1], rhs[0], rhs[1], epsilon);

        if (!par)
        {
            return {};
        }

        const auto [a, b] = *par;

        if (contains_param(Tag1{}, a) && contains_param(Tag2{}, b))
        {
            return interpolate(a, lhs[0], lhs[1]);
        }
        return {};
    }
};

static constexpr inline auto intersection = intersection_fn{};

struct projection_fn
{
    template <class T, std::size_t D>
    auto operator()(const vector<T, D>& lhs, const vector<T, D>& rhs) const -> decltype(rhs * (dot(rhs, lhs) / norm(rhs)))
    {
        return rhs * (dot(rhs, lhs) / norm(rhs));
    }

    template <class T, std::size_t D, class Tag, class E = T>
    auto operator()(const vector<T, D>& point, const linear_shape<Tag, T, D>& shape, E epsilon = {}) const
        -> std::optional<vector<T, D>>
    {
        const auto p0 = shape[0];
        const auto p1 = shape[1];

        const auto result = p0 + (*this)(point - p0, p1 - p0);

        const auto t = get_line_intersection_parameter(p0, p1, result, epsilon);

        if (t && contains_param(Tag{}, *t))
        {
            return result;
        }

        return {};
    }
};

static constexpr inline auto projection = projection_fn{};

struct rejection_fn
{
    template <class T, std::size_t D>
    auto operator()(const vector<T, D>& lhs, const vector<T, D>& rhs) const -> decltype(lhs - projection(lhs, rhs))
    {
        return lhs - projection(lhs, rhs);
    }
};

static constexpr inline auto rejection = rejection_fn{};

struct perpendicular_fn
{
    template <class T>
    auto operator()(const vector<T, 2>& value) const -> vector<T, 2>
    {
        return vector<T, 2>{ -value[1], value[0] };
    }

    template <class Tag, class T>
    auto operator()(const linear_shape<Tag, T, 2>& value, const vector<T, 2>& origin) const -> linear_shape<Tag, T, 2>
    {
        return { origin, origin + (*this)(value[1] - value[0]) };
    }

    template <class Tag, class T>
    auto operator()(const linear_shape<Tag, T, 2>& value) const -> linear_shape<Tag, T, 2>
    {
        return (*this)(value, value[0]);
    }
};

static constexpr inline auto perpendicular = perpendicular_fn{};

struct altitude_fn
{
    template <typename T>
    auto operator()(const triangle_2d<T>& value, std::size_t index) const -> segment_2d<T>
    {
        static const T epsilon = T(0.1);

        const auto v = value[(index + 0) % 3];

        const auto p = projection(v, line_2d<T>{ value[(index + 1) % 3], value[(index + 2) % 3] }, epsilon);

        return { v, *p };
    }
};

static constexpr inline auto altitude = altitude_fn{};

struct centroid_fn
{
    template <typename T>
    auto operator()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        return std::accumulate(std::begin(value), std::end(value), vector_2d<T>{}) / 3;
    }
};

static constexpr inline auto centroid = centroid_fn{};

struct orthocenter_fn
{
    template <typename T>
    auto operator()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        static const T epsilon = T(0.0001);

        return *intersection(make_line(altitude(value, 0)), make_line(altitude(value, 1)), epsilon);
    }
};

static constexpr inline auto orthocenter = orthocenter_fn{};

struct circumcenter_fn
{
    template <typename T>
    auto operator()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        static T epsilon = T(0.0001);

        const auto s0 = segment_2d<T>{ value[0], value[1] };
        const auto s1 = segment_2d<T>{ value[1], value[2] };

        return *intersection(make_line(perpendicular(s0, center(s0))), make_line(perpendicular(s1, center(s1))), epsilon);
    }
};

static constexpr inline auto circumcenter = circumcenter_fn{};

struct incenter_fn
{
    template <typename T>
    auto operator()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        T sum = T(0);

        vector_2d<T> result;

        for (size_t i = 0; i < 3; ++i)
        {
            const auto side_length = length(segment_2d<T>{ value[(i + 0) % 3], value[(i + 1) % 3] });

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
    auto operator()(const triangle_2d<T>& triangle) const -> circle_2d<T>
    {
        static const T epsilon = T(0.1);

        const auto center = incenter(triangle);
        const auto radius = distance(center, *projection(center, segment_2d<T>{ triangle[0], triangle[1] }, epsilon));

        return circle<T>{ center, radius };
    }
};

static constexpr inline auto incircle = incircle_fn{};

struct circumcircle_fn
{
    template <class T>
    auto operator()(const triangle_2d<T>& triangle) const -> circle_2d<T>
    {
        const auto center = circumcenter(triangle);
        auto radius = distance(center, triangle[0]);

        return circle<T>{ center, radius };
    }
};

static constexpr inline auto circumcircle = circumcircle_fn{};

}  // namespace detail

using detail::altitude;
using detail::angle;
using detail::center;
using detail::centroid;
using detail::circumcenter;
using detail::circumcircle;
using detail::contains;
using detail::cross;
using detail::distance;
using detail::dot;
using detail::incenter;
using detail::incircle;
using detail::interpolate;
using detail::intersection;
using detail::intersects;
using detail::length;
using detail::lower;
using detail::norm;
using detail::orthocenter;
using detail::perpendicular;
using detail::projection;
using detail::rejection;
using detail::size;
using detail::unit;
using detail::upper;

}  // namespace alg
}  // namespace ferrugo
