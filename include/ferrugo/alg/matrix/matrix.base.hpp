#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <type_traits>

namespace ferrugo
{
namespace alg
{

namespace detail
{
struct raw_t
{
};

static constexpr inline auto raw = raw_t{};
}  // namespace detail

template <class T, std::size_t R, std::size_t C>
class matrix
{
public:
    using data_type = std::array<T, R * C>;

    using size_type = size_t;

    using value_type = T;
    using const_reference = const T&;
    using reference = T&;
    using const_pointer = const T*;
    using pointer = T*;
    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

    constexpr matrix()
    {
        std::fill(begin(), end(), value_type{});
    }

    constexpr explicit matrix(detail::raw_t)
    {
    }

    constexpr matrix(std::initializer_list<value_type> init)
    {
        std::copy(std::begin(init), std::end(init), begin());
    }

    constexpr matrix(const matrix& other)
    {
        std::copy(std::begin(other), std::end(other), begin());
    }

    template <class U>
    constexpr matrix(const matrix<U, R, C>& other)
    {
        std::transform(std::begin(other), std::end(other), begin(), [](U v) -> T { return static_cast<T>(v); });
    }

    constexpr matrix& operator=(matrix other)
    {
        std::swap(m_data, other.m_data);
        return *this;
    }

    constexpr size_type row_count() const
    {
        return R;
    }

    constexpr size_type col_count() const
    {
        return C;
    }

    constexpr size_type size() const
    {
        return m_data.size();
    }

    constexpr const_reference operator()(size_type r, size_type c) const
    {
        return (*this)[r * col_count() + c];
    }

    constexpr reference operator()(size_type r, size_type c)
    {
        return (*this)[r * col_count() + c];
    }

    constexpr const_reference operator[](size_type index) const
    {
        return m_data[index];
    }

    constexpr reference operator[](size_type index)
    {
        return m_data[index];
    }

    constexpr const_iterator begin() const
    {
        return m_data.begin();
    }

    constexpr const_iterator end() const
    {
        return m_data.end();
    }

    constexpr iterator begin()
    {
        return m_data.begin();
    }

    constexpr iterator end()
    {
        return m_data.end();
    }

    template <std::size_t R_ = R, std::size_t C_ = C, class = std::enable_if_t<(R_ == 1 && C_ >= 1)>>
    constexpr const_reference x() const
    {
        return std::get<0>(m_data);
    }

    template <std::size_t R_ = R, std::size_t C_ = C, class = std::enable_if_t<(R_ == 1 && C_ >= 2)>>
    constexpr const_reference y() const
    {
        return std::get<1>(m_data);
    }

    template <std::size_t R_ = R, std::size_t C_ = C, class = std::enable_if_t<(R_ == 1 && C_ >= 3)>>
    constexpr const_reference z() const
    {
        return std::get<2>(m_data);
    }

    data_type m_data;
};

template <class T, std::size_t D>
using square_matrix = matrix<T, D, D>;

template <class T, std::size_t D>
using vector = matrix<T, 1, D>;

template <class T>
using square_matrix_2d = square_matrix<T, 3>;

template <class T>
using square_matrix_3d = square_matrix<T, 4>;

template <class T>
using vector_2d = vector<T, 2>;

template <class T>
using vector_3d = vector<T, 3>;

template <class T, std::size_t R, std::size_t C>
std::ostream& operator<<(std::ostream& os, const matrix<T, R, C>& item)
{
    os << "[";

    for (std::size_t r = 0; r < item.row_count(); ++r)
    {
        os << "[";

        for (std::size_t c = 0; c < item.col_count(); ++c)
        {
            if (c != 0)
            {
                os << " ";
            }

            os << item(r, c);
        }

        os << "]";
    }

    os << "]";

    return os;
}

template <class T, std::size_t D>
std::ostream& operator<<(std::ostream& os, const vector<T, D>& item)
{
    os << "[";

    for (std::size_t d = 0; d < item.size(); ++d)
    {
        if (d != 0)
        {
            os << " ";
        }

        os << item[d];
    }

    os << "]";

    return os;
}

template <std::size_t Index, class T, std::size_t R, std::size_t C>
constexpr auto get(const matrix<T, R, C>& m) -> typename matrix<T, R, C>::const_reference
{
    return std::get<Index>(m.m_data);
}

template <std::size_t Index, class T, std::size_t R, std::size_t C>
constexpr auto get(matrix<T, R, C>& m) -> typename matrix<T, R, C>::reference
{
    return std::get<Index>(m.m_data);
}

template <std::size_t Row, std::size_t Col, class T, std::size_t R, std::size_t C>
constexpr auto get(const matrix<T, R, C>& m) -> typename matrix<T, R, C>::const_reference
{
    return std::get<Row * C + Col>(m.m_data);
}

template <std::size_t Row, std::size_t Col, class T, std::size_t R, std::size_t C>
constexpr auto get(matrix<T, R, C>& m) -> typename matrix<T, R, C>::reference
{
    return std::get<Row * C + Col>(m.m_data);
}

template <class T>
constexpr auto vec(T x, T y) -> vector<T, 2>
{
    return vector<T, 2>{ x, y };
}

template <class T>
constexpr auto vec(T x, T y, T z) -> vector<T, 3>
{
    return vector<T, 3>{ x, y, z };
}

}  // namespace alg
}  // namespace ferrugo

namespace std
{

template <class T, size_t D>
struct tuple_size<::ferrugo::alg::vector<T, D>> : std::integral_constant<size_t, D>
{
};

template <size_t Index, class T, size_t D>
struct tuple_element<Index, ::ferrugo::alg::vector<T, D>>
{
    using type = decltype(::ferrugo::alg::get<Index>(std::declval<::ferrugo::alg::vector<T, D>>()));
};

template <size_t Index, class T, size_t D>
const auto& get(const ::ferrugo::alg::vector<T, D>& item)
{
    return ::ferrugo::alg::get<Index>(item);
};

}  // namespace std
