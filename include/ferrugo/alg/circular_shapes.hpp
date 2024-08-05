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

    friend std::ostream& operator<<(std::ostream& os, const circular_shape& item)
    {
        return os << "(circle " << item.center << " " << item.radius << ")";
    }
};

template <class T>
using circle = circular_shape<T, 2>;

template <class T>
using sphere = circular_shape<T, 3>;

template <class T>
using circle_2d = circle<T>;

template <class T, class U, std::size_t D>
auto operator+=(circular_shape<T, D>& lhs, const vector<U, D>& rhs) -> circular_shape<T, D>&
{
    lhs.center += rhs;
    return lhs;
}

template <class T, class U, std::size_t D>
auto operator+(circular_shape<T, D> lhs, const vector<U, D>& rhs) -> circular_shape<T, D>
{
    lhs.center += rhs;
    return lhs;
}

template <class T, class U, std::size_t D>
auto operator-=(circular_shape<T, D>& lhs, const vector<U, D>& rhs) -> circular_shape<T, D>&
{
    lhs.center -= rhs;
    return lhs;
}

template <class T, class U, std::size_t D>
auto operator-(circular_shape<T, D> lhs, const vector<U, D>& rhs) -> circular_shape<T, D>
{
    lhs.center -= rhs;
    return lhs;
}

}  // namespace alg
}  // namespace ferrugo
