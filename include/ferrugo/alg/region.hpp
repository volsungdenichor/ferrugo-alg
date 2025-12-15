#pragma once

#include <ferrugo/alg/interval.hpp>
#include <ferrugo/alg/matrix.hpp>

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
using region_3d = region<T, 3>;

template <class T>
using rect = region_2d<T>;

template <class T>
using cuboid = region_3d<T>;

template <class T, class U, std::size_t D>
bool operator==(const region<T, D>& lhs, const region<U, D>& rhs)
{
    for (std::size_t d = 0; d < D; ++d)
    {
        if (lhs[d] != rhs[d])
        {
            return false;
        }
    }
    return true;
}

template <class T, class U, std::size_t D>
bool operator!=(const region<T, D>& lhs, const region<U, D>& rhs)
{
    return !(lhs == rhs);
}

template <class T, class U, std::size_t D, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator+=(region<T, D>& lhs, const vector<U, D>& rhs) -> region<T, D>&
{
    for (std::size_t d = 0; d < D; ++d)
    {
        lhs[d] += rhs[d];
    }
    return lhs;
}

template <class T, class U, std::size_t D, class Res = std::invoke_result_t<std::plus<>, T, U>>
auto operator+(const region<T, D>& lhs, const vector<U, D>& rhs) -> region<Res, D>
{
    region<Res, D> result{};
    for (std::size_t d = 0; d < D; ++d)
    {
        result[d] = lhs[d] + rhs[d];
    }
    return result;
}

template <class T, class U, std::size_t D, class Res = std::invoke_result_t<std::minus<>, T, U>>
auto operator-=(region<T, D>& lhs, const vector<U, D>& rhs) -> region<T, D>&
{
    for (std::size_t d = 0; d < D; ++d)
    {
        lhs[d] -= rhs[d];
    }
    return lhs;
}

template <class T, class U, std::size_t D, class Res = std::invoke_result_t<std::minus<>, T, U>>
auto operator-(const region<T, D>& lhs, const vector<U, D>& rhs) -> region<Res, D>
{
    region<Res, D> result{};
    for (std::size_t d = 0; d < D; ++d)
    {
        result[d] = lhs[d] - rhs[d];
    }
    return result;
}

}  // namespace alg
}  // namespace ferrugo