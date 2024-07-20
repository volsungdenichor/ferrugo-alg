#pragma once

#include <ferrugo/alg/matrix/matrix.base.hpp>
#include <optional>
#include <stdexcept>

namespace ferrugo
{
namespace alg
{

namespace detail
{

struct minor_fn
{
    template <class T, std::size_t R, std::size_t C>
    auto operator()(const matrix<T, R, C>& item, std::size_t row, std::size_t col) const -> matrix<T, R - 1, C - 1>
    {
        static_assert(R > 1, "minor: invalid row.");
        static_assert(C > 1, "minor: invalid col.");

        if (row >= R || col >= C)
        {
            throw std::runtime_error{ "minor: invalid row or column" };
        }

        matrix<T, R - 1, C - 1> result{ raw };

        for (std::size_t r = 0; r < R; ++r)
        {
            for (std::size_t c = 0; c < C; ++c)
            {
                result(r, c) = item(r + (r < row ? 0 : 1), c + (c < col ? 0 : 1));
            }
        }

        return result;
    }
};

static constexpr inline auto minor = minor_fn{};

struct determinant_fn
{
    template <class T>
    auto operator()(const square_matrix<T, 1>& item) const
    {
        return get<0, 0>(item);
    }

    template <class T>
    auto operator()(const square_matrix<T, 2>& item) const
    {
        return get<0, 0>(item) * get<1, 1>(item) - get<0, 1>(item) * get<1, 0>(item);
    }

    template <class T>
    auto operator()(const square_matrix<T, 3>& item) const
    {
        // clang-format off
        return
            + get<0, 0>(item) * get<1, 1>(item) * get<2, 2>(item)
            + get<0, 1>(item) * get<1, 2>(item) * get<2, 0>(item)
            + get<0, 2>(item) * get<1, 0>(item) * get<2, 1>(item)
            - get<0, 2>(item) * get<1, 1>(item) * get<2, 0>(item)
            - get<0, 0>(item) * get<1, 2>(item) * get<2, 1>(item)
            - get<0, 1>(item) * get<1, 0>(item) * get<2, 2>(item);
        // clang-format on
    }

    template <class T, std::size_t D>
    auto operator()(const square_matrix<T, D>& item) const
    {
        auto sum = T{};

        for (std::size_t i = 0; i < D; ++i)
        {
            sum += (i % 2 == 0 ? 1 : -1) * item(0, i) * (*this)(minor(item, 0, i));
        }

        return sum;
    }
};

static constexpr inline auto determinant = determinant_fn{};

struct invert_fn
{
    template <class T, std::size_t D>
    auto operator()(const square_matrix<T, D>& value) const -> std::optional<square_matrix<T, D>>
    {
        const auto det = determinant(value);

        if (!det)
        {
            return {};
        }

        square_matrix<T, D> result{ raw };

        for (std::size_t r = 0; r < D; ++r)
        {
            for (std::size_t c = 0; c < D; ++c)
            {
                result(c, r) = T((r + c) % 2 == 0 ? 1 : -1) * determinant(minor(value, r, c)) / det;
            }
        }

        return result;
    }
};

static constexpr inline auto invert = invert_fn{};

struct transpose_fn
{
    template <class T, std::size_t R, std::size_t C>
    auto operator()(const matrix<T, R, C>& item) const -> matrix<T, C, R>
    {
        matrix<T, C, R> result{ raw };

        for (size_t r = 0; r < R; ++r)
        {
            for (size_t c = 0; c < C; ++c)
            {
                result(c, r) = item(r, c);
            }
        }

        return result;
    }
};

static constexpr inline auto transpose = transpose_fn{};

}  // namespace detail

using detail::determinant;
using detail::invert;
using detail::minor;
using detail::transpose;

}  // namespace alg
}  // namespace ferrugo