#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <ferrugo/alg/operations.hpp>

using namespace ferrugo;

TEST_CASE("region", "[operations]")
{
    const auto region = alg::region_2d<float>{ alg::interval<float>{ 0, 5 }, alg::interval<float>{ 1, 3 } };
    REQUIRE(alg::lower(region) == alg::vector_2d<float>{ 0, 1 });
    REQUIRE(alg::upper(region) == alg::vector_2d<float>{ 5, 3 });
    REQUIRE(alg::size(region) == alg::vector_2d<float>{ 5, 2 });
    REQUIRE(alg::center(region) == alg::vector_2d<float>{ 2.5, 2 });
}