#pragma once

#include <ferrugo/alg/matrix.hpp>
#include <vector>

namespace ferrugo
{
namespace alg
{
template <class T, std::size_t D, std::size_t N>
struct polygon_base : std::array<vector<T, D>, N>
{
    using base_t = std::array<vector<T, D>, N>;

    friend std::ostream& operator<<(std::ostream& os, const polygon_base& item)
    {
        os << "(";
        for (std::size_t n = 0; n < item.size(); ++n)
        {
            if (n != 0)
            {
                os << " ";
            }
            os << item[n];
        }
        os << ")";
        return os;
    }
};

template <class T, std::size_t D>
using triangle = polygon_base<T, D, 3>;

template <class T, std::size_t D>
using quad = polygon_base<T, D, 4>;

template <class T>
using triangle_2d = triangle<T, 2>;

template <class T>
using quad_2d = quad<T, 2>;

template <class T, class U, std::size_t D, std::size_t N>
auto operator+=(polygon_base<T, D, N>& lhs, const vector<U, D>& rhs) -> polygon_base<T, D, N>&
{
    std::transform(std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::plus<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class T, class U, std::size_t D, std::size_t N, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator+(const polygon_base<T, D, N>& lhs, const vector<U, D>& rhs) -> polygon_base<Res, D, N>
{
    polygon_base<Res, D, N> result{};
    std::transform(std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::plus<>{}, std::placeholders::_1, rhs));
    return result;
}

template <class T, class U, std::size_t D, std::size_t N>
auto operator-=(polygon_base<T, D, N>& lhs, const vector<U, D>& rhs) -> polygon_base<T, D, N>&
{
    std::transform(std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::minus<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class T, class U, std::size_t D, std::size_t N, class Res = std::invoke_result_t<std::minus<>, T, U>>
auto operator-(const polygon_base<T, D, N>& lhs, const vector<U, D>& rhs) -> polygon_base<Res, D, N>
{
    polygon_base<Res, D, N> result{};
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::minus<>{}, std::placeholders::_1, rhs));
    return result;
}

template <class T, class U, std::size_t D, std::size_t N>
auto operator*=(polygon_base<T, D, N>& lhs, const square_matrix<U, D + 1>& rhs) -> polygon_base<T, D, N>&
{
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::multiplies<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class T, class U, std::size_t D, std::size_t N, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator*(const polygon_base<T, D, N>& lhs, const square_matrix<U, D + 1>& rhs) -> polygon_base<Res, D, N>
{
    polygon_base<Res, D, N> result{};
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::multiplies<>{}, std::placeholders::_1, rhs));
    return result;
}

template <class T, class U, std::size_t D, std::size_t N, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator*(const square_matrix<U, D + 1>& lhs, const polygon_base<T, D, N>& rhs) -> polygon_base<Res, D, N>
{
    return rhs * lhs;
}

}  // namespace alg
}  // namespace ferrugo