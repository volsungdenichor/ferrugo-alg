#pragma once

#include <ferrugo/alg/matrix.hpp>

namespace ferrugo
{
namespace alg
{
template <class T, std::size_t D>
struct circular_shape
{
    vector<T, D> center;
    T radius;
};

template <class T>
using circle = circular_shape<T, 2>;

template <class T>
using shpere = circular_shape<T, 3>;

template <class T>
using circle_2d = circle<T>;

}  // namespace alg
}  // namespace ferrugo
