#pragma once

#include <ferrugo/alg/math.hpp>
#include <ferrugo/alg/matrix/matrix.base.hpp>
#include <ferrugo/alg/matrix/matrix.operators.hpp>

namespace ferrugo
{
namespace alg
{

namespace detail
{

struct identity_fn
{
    template <size_t D, class T = double>
    square_matrix<T, D> create() const
    {
        square_matrix<T, D> result;

        for (size_t r = 0; r < D; ++r)
        {
            for (size_t c = 0; c < D; ++c)
            {
                result(r, c) = r == c ? T(1) : T(0);
            }
        }

        return result;
    }

    template <class T, std::size_t D>
    operator square_matrix<T, D>() const
    {
        return create<D, T>();
    }
};

static constexpr inline auto identity = identity_fn{};

struct scale_fn
{
    template <class T>
    square_matrix_2d<T> operator()(const vector_2d<T>& scale) const
    {
        square_matrix_2d<T> result = identity;

        get<0, 0>(result) = get<0>(scale);
        get<1, 1>(result) = get<1>(scale);

        return result;
    }

    template <class T>
    square_matrix_3d<T> operator()(const vector_3d<T>& scale) const
    {
        square_matrix_3d<T> result = identity;

        get<0, 0>(result) = get<0>(scale);
        get<1, 1>(result) = get<1>(scale);
        get<2, 2>(result) = get<2>(scale);

        return result;
    }

    template <class T>  
    square_matrix_2d<T> operator()(T x, T y) const
    {
        return (*this)(vector_2d<T>{ x, y });
    }

    template <class T>
    square_matrix_3d<T> operator()(T x, T y, T z) const
    {
        return (*this)(vector_3d<T>{ x, y, z });
    }
};

struct rotation_fn
{
    template <class T>
    square_matrix_2d<T> operator()(T angle) const
    {
        square_matrix_2d<T> result = identity;

        const auto c = cos(angle);
        const auto s = sin(angle);

        get<0, 0>(result) = c;
        get<0, 1>(result) = s;
        get<1, 0>(result) = -s;
        get<1, 1>(result) = c;

        return result;
    }
};

struct translation_fn
{
    template <class T>
    square_matrix_2d<T> operator()(const vector_2d<T>& offset) const
    {
        square_matrix_2d<T> result = identity;

        get<2, 0>(result) = get<0>(offset);
        get<2, 1>(result) = get<1>(offset);

        return result;
    }

    template <class T>
    square_matrix_3d<T> operator()(const vector_3d<T>& offset) const
    {
        square_matrix_3d<T> result = identity;

        get<3, 0>(result) = get<0>(offset);
        get<3, 1>(result) = get<1>(offset);
        get<3, 2>(result) = get<2>(offset);

        return result;
    }

    template <class T>
    square_matrix_2d<T> operator()(T x, T y) const
    {
        return (*this)(vector_2d<T>{ x, y });
    }

    template <class T>
    square_matrix_3d<T> operator()(T x, T y, T z) const
    {
        return (*this)(vector_3d<T>{ x, y, z });
    }
};

}  // namespace detail

using detail::identity;

static constexpr inline auto scale = detail::scale_fn{};
static constexpr inline auto translation = detail::translation_fn{};
static constexpr inline auto rotation = detail::rotation_fn{};

}  // namespace alg
}  // namespace ferrugo