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
struct polygon_base<T, D, 0> : std::vector<vector<T, D>>
{
    using base_t = std::vector<vector<T, D>>;

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

template <class T, std::size_t D>
using polygon = polygon_base<T, D, 0>;

template <class T>
using polygon_2d = polygon<T, 2>;

}  // namespace alg
}  // namespace ferrugo