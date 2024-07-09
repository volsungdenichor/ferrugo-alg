#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <ferrugo/mat/matrix.hpp>

using namespace ferrugo;

TEST_CASE("matrix - access", "[matrix]")
{
    mat::vector_2d<int> v{ 3, 2 };
    const auto [x, y] = v;
    REQUIRE(x == 3);
    REQUIRE(y == 2);
    REQUIRE(v.x() == 3);
    REQUIRE(v.y() == 2);
}

TEST_CASE("matrix - comparison", "[matrix]")
{
    mat::vector_2d<int> a{ 3, 2 };
    mat::vector_2d<int> b{ 3, 3 };
    REQUIRE(a != b);
    REQUIRE(a == a);
}

TEST_CASE("vector - length", "[vector]")
{
    mat::vector_2d<int> a{ 1, 1 };
    REQUIRE_THAT(mat::length(a), Catch::Matchers::WithinRel(1.4142, 0.001));
}

TEST_CASE("vector - dot", "[vector]")
{
    mat::vector_2d<int> a{ 2, 3 };
    mat::vector_2d<int> b{ 4, 5 };
    REQUIRE(mat::dot(a, b) == 23);
}

TEST_CASE("vector - norm", "[vector]")
{
    mat::vector_2d<int> a{ 2, 3 };
    REQUIRE(mat::norm(a) == 13);
}
