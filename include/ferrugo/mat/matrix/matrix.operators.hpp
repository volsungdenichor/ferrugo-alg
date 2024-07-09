#pragma once

#include <ferrugo/mat/matrix/matrix.base.hpp>
#include <functional>

namespace ferrugo
{
namespace mat
{

template <class T, class U, std::size_t R, std::size_t C>
bool operator==(const matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs)
{
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
}

template <class T, class U, std::size_t R, std::size_t C>
bool operator!=(const matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs)
{
    return !(lhs == rhs);
}

template <class T, std::size_t R, std::size_t C>
auto operator+(const matrix<T, R, C>& item) -> matrix<T, R, C>
{
    return item;
}

template <class T, std::size_t R, std::size_t C>
auto operator-(const matrix<T, R, C>& item) -> matrix<T, R, C>
{
    matrix<T, R, C> result{ detail::raw };
    std::transform(std::begin(item), std::end(item), std::begin(result), std::negate<>{});
    return result;
}

template <class T, class U, std::size_t R, std::size_t C, class = std::invoke_result_t<std::plus<>, T, U>>
auto operator+=(matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs) -> matrix<T, R, C>&
{
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs), std::begin(lhs), std::plus<>{});
    return lhs;
}

template <class T, class U, std::size_t R, std::size_t C, class = std::invoke_result_t<std::minus<>, T, U>>
auto operator-=(matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs) -> matrix<T, R, C>&
{
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs), std::begin(lhs), std::minus<>{});
    return lhs;
}

template <class T, class U, std::size_t R, std::size_t C, class = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*=(matrix<T, R, C>& lhs, U rhs) -> matrix<T, R, C>&
{
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::multiplies<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class T, class U, std::size_t R, std::size_t C, class = std::invoke_result_t<std::divides<>, T, U>>
auto operator/=(matrix<T, R, C>& lhs, U rhs) -> matrix<T, R, C>&
{
    std::transform(std::begin(lhs), std::end(lhs), std::begin(lhs), std::bind(std::divides<>{}, std::placeholders::_1, rhs));
    return lhs;
}

template <class T, class U, std::size_t R, std::size_t C, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator+(const matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs) -> matrix<Res, R, C>
{
    matrix<Res, R, C> result{ detail::raw };
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs), std::begin(result), std::plus<>{});
    return result;
}

template <class T, class U, std::size_t R, std::size_t C, class Res = std::invoke_result_t<std::minus<>, T, U>>
auto operator-(const matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs) -> matrix<Res, R, C>
{
    matrix<Res, R, C> result{ detail::raw };
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs), std::begin(result), std::minus<>{});
    return result;
}

template <class T, class U, std::size_t R, std::size_t C, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(const matrix<T, R, C>& lhs, U rhs) -> matrix<Res, R, C>
{
    matrix<Res, R, C> result{ detail::raw };
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::multiplies<>{}, std::placeholders::_1, rhs));
    return result;
}

template <class T, class U, std::size_t R, std::size_t C, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(T lhs, const matrix<U, R, C>& rhs) -> matrix<Res, R, C>
{
    return rhs * lhs;
}

template <class T, class U, std::size_t R, std::size_t C, class Res = std::invoke_result_t<std::divides<>, T, U>>
auto operator/(const matrix<T, R, C>& lhs, U rhs) -> matrix<Res, R, C>
{
    matrix<Res, R, C> result{ detail::raw };
    std::transform(
        std::begin(lhs), std::end(lhs), std::begin(result), std::bind(std::divides<>{}, std::placeholders::_1, rhs));
    return result;
}

template <
    class T,
    class U,
    std::size_t R,
    std::size_t D,
    std::size_t C,
    class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(const matrix<T, R, D>& lhs, const matrix<U, D, C>& rhs) -> matrix<Res, R, C>
{
    matrix<Res, R, C> result;

    for (std::size_t r = 0; r < lhs.row_count(); ++r)
    {
        for (std::size_t c = 0; c < rhs.col_count(); ++c)
        {
            Res sum = {};

            for (std::size_t i = 0; i < D; ++i)
            {
                sum += lhs(r, i) * rhs(i, c);
            }

            result(r, c) = sum;
        }
    }

    return result;
}

template <class T, class U, std::size_t D, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(const vector<T, D>& lhs, const square_matrix<U, D + 1>& rhs) -> vector<Res, D>
{
    vector<Res, D> result;

    for (std::size_t d = 0; d < lhs.size(); ++d)
    {
        Res sum = static_cast<Res>(rhs(D, d));

        for (std::size_t i = 0; i < D; ++i)
        {
            sum += lhs[i] * rhs(i, d);
        }

        result[d] = sum;
    }

    return result;
}

template <class T, class U, std::size_t D, class Res = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*(const square_matrix<T, D + 1>& lhs, const vector<U, D>& rhs) -> vector<Res, D>
{
    return rhs * lhs;
}

template <class T, class U, std::size_t D, class = std::invoke_result_t<std::multiplies<>, T, U>>
auto operator*=(vector<T, D>& lhs, const square_matrix<U, D + 1>& rhs) -> vector<T, D>&
{
    return lhs = lhs * rhs;
}

}  // namespace mat
}  // namespace ferrugo
