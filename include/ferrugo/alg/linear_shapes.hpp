#pragma once

#include <ferrugo/alg/matrix.hpp>

namespace ferrugo
{
namespace alg
{
namespace detail
{

struct ray_tag
{
};
struct line_tag
{
};
struct segment_tag
{
};

template <class Tag, class T, std::size_t D>
struct linear_shape : std::array<vector<T, D>, 2>
{
    using base_t = std::array<vector<T, D>, 2>;

    using base_t::base_t;

    linear_shape(vector<T, D> p0, vector<T, D> p1) : base_t{ { p0, p1 } }
    {
    }
};

template <class T, std::size_t D>
using line = detail::linear_shape<detail::line_tag, T, D>;

template <class T, std::size_t D>
using ray = detail::linear_shape<detail::ray_tag, T, D>;

template <class T, std::size_t D>
using segment = detail::linear_shape<detail::segment_tag, T, D>;

template <class T, std::size_t D>
std::ostream& operator<<(std::ostream& os, const line<T, D>& item)
{
    return os << "(line " << item[0] << " (dir " << (item[1] - item[0]) << "))";
}

template <class T, std::size_t D>
std::ostream& operator<<(std::ostream& os, const ray<T, D>& item)
{
    return os << "(ray " << item[0] << " (dir " << (item[1] - item[0]) << "))";
}

template <class T, std::size_t D>
std::ostream& operator<<(std::ostream& os, const segment<T, D>& item)
{
    return os << "(segment " << item[0] << " " << item[1] << ")";
}

template <class Tag, class T, class U, std::size_t D>
auto operator+=(linear_shape<Tag, T, D>& lhs, const vector<U, D>& rhs) -> linear_shape<Tag, T, D>&
{
    std::transform(std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::plus<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class Tag, class T, class U, std::size_t D, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator+(const linear_shape<Tag, T, D>& lhs, const vector<U, D>& rhs) -> linear_shape<Tag, Res, D>
{
    linear_shape<Tag, Res, D> result{};
    std::transform(std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::plus<>{}, std::placeholders::_1, rhs));
    return result;
}

template <class Tag, class T, class U, std::size_t D>
auto operator-=(linear_shape<Tag, T, D>& lhs, const vector<U, D>& rhs) -> linear_shape<Tag, T, D>&
{
    std::transform(std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::minus<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class Tag, class T, class U, std::size_t D, class Res = std::invoke_result_t<std::minus<>, T, U>>
auto operator-(const linear_shape<Tag, T, D>& lhs, const vector<U, D>& rhs) -> linear_shape<Tag, Res, D>
{
    linear_shape<Tag, Res, D> result{};
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::minus<>{}, std::placeholders::_1, rhs));
    return result;
}

template <class Tag, class T, class U, std::size_t D>
auto operator*=(linear_shape<Tag, T, D>& lhs, const square_matrix<U, D + 1>& rhs) -> linear_shape<Tag, T, D>&
{
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::multiplies<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class Tag, class T, class U, std::size_t D, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(const linear_shape<Tag, T, D>& lhs, const square_matrix<U, D + 1>& rhs) -> linear_shape<Tag, Res, D>
{
    linear_shape<Tag, Res, D> result{};
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::multiplies<>{}, std::placeholders::_1, rhs));
    return result;
}

template <class Tag, class T, class U, std::size_t D, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(const square_matrix<U, D + 1>& lhs, const linear_shape<Tag, T, D>& rhs) -> linear_shape<Tag, Res, D>
{
    return rhs * lhs;
}

}  // namespace detail

using detail::line;
using detail::ray;
using detail::segment;

template <class T>
using line_2d = line<T, 2>;

template <class T>
using ray_2d = ray<T, 2>;

template <class T>
using segment_2d = segment<T, 2>;

}  // namespace alg
};  // namespace ferrugo
