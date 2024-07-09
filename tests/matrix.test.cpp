#include <catch2/catch_test_macros.hpp>
#include <ferrugo/mat/matrix.hpp>

using namespace ferrugo;

TEST_CASE("matrix - access", "[matrix]")
{
    mat::vector_2d<int> v{ 3, 2 };
    const auto [x, y] = v;
    REQUIRE(x == 3);
    REQUIRE(y == 2);
}

TEST_CASE("matrix - comparison", "[matrix]")
{
    mat::vector_2d<int> a{ 3, 2 };
    mat::vector_2d<int> b{ 3, 3 };
    REQUIRE(a != b);
    REQUIRE(a == a);
}
