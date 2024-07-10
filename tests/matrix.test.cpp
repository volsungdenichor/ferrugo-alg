#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <ferrugo/alg/matrix.hpp>

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
}

TEST_CASE("vector - length", "[vector]")
{
    alg::vector_2d<int> a{ 1, 1 };
    REQUIRE_THAT(alg::length(a), Catch::Matchers::WithinRel(1.4142, 0.001));
}

TEST_CASE("vector - dot", "[vector]")
{
    alg::vector_2d<int> a{ 2, 3 };
    alg::vector_2d<int> b{ 4, 5 };
    REQUIRE(alg::dot(a, b) == 23);
}

TEST_CASE("vector - norm", "[vector]")
{
    alg::vector_2d<int> a{ 2, 3 };
    REQUIRE(alg::norm(a) == 13);
}
