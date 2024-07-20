#pragma once

#include <ferrugo/alg/interval.hpp>

namespace ferrugo
{
namespace alg
{
template <class T, std::size_t D>
struct region : std::array<interval<T>, D>
{
    using base_t = std::array<interval<T>, D>;

    friend std::ostream& operator<<(std::ostream& os, const region& item)
    {
        os << "(";
        for (std::size_t d = 0; d < D; ++d)
        {
            if (d != 0)
            {
                os << " ";
            }
            os << item[d];
        }
        os << ")";
        return os;
    }
};

template <class T>
using region_2d = region<T, 2>;

template <class T>
using region_3d = region<T, 2>;

template <class T>
using rect = region_2d<T>;

template <class T>
using cuboid = region_3d<T>;

}  // namespace alg
}  // namespace ferrugo