#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <ferrugo/alg/interval.hpp>
#include <ferrugo/alg/linear.hpp>
#include <ferrugo/alg/matrix.hpp>
#include <ferrugo/alg/polygon.hpp>
#include <ferrugo/alg/region.hpp>

using namespace ferrugo;

TEST_CASE("matrix - access", "[matrix]")
{
    alg::vector_2d<int> v{ 3, 2 };
    const auto [x, y] = v;
    REQUIRE(x == 3);
    REQUIRE(y == 2);
    REQUIRE(v.x() == 3);
    REQUIRE(v.y() == 2);
}

TEST_CASE("matrix - comparison", "[matrix]")
{
    alg::vector_2d<int> a{ 3, 2 };
    alg::vector_2d<int> b{ 3, 3 };
    REQUIRE(a != b);
    REQUIRE(a == a);

    std::cout << alg::minor(alg::square_matrix_2d<int>{}, 0, 0) << std::endl;
    std::cout << alg::determinant(alg::square_matrix_3d<int>{}) << std::endl;
    std::cout << *alg::invert(alg::square_matrix_3d<int>{}) << std::endl;
}
