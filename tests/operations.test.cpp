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

TEST_CASE("region operations", "[operators]")
{
    const auto region = alg::region_2d<float>{ alg::interval<float>{ 0, 5 }, alg::interval<float>{ 1, 3 } };
    REQUIRE(region - alg::vector_2d<float>{ 1, 5 } == alg::region_2d<float>{});
}

TEST_CASE("segment operations", "[operators]")
{
    const auto seg = alg::segment_2d<float>{ alg::vec(0.F, 5.F), alg::vec(1.F, 3.F) };
    std::cout << (seg + alg::vec(10, 10)) << std::endl;
    std::cout << (seg - alg::vec(10, 10)) << std::endl;
    std::cout << (seg * alg::rotation(10.F)) << std::endl;
    std::cout << (alg::rotation(10.F) * seg) << std::endl;
}

TEST_CASE("triangle operations", "[operators]")
{
    const auto shape = alg::triangle_2d<float>{ alg::vec(0.F, 5.F), alg::vec(1.F, 3.F), alg::vec(2.F, 0.F) };
    std::cout << (shape + alg::vec(10, 10)) << std::endl;
    std::cout << (shape - alg::vec(10, 10)) << std::endl;
    std::cout << (shape * alg::rotation(0.1F)) << std::endl;
    std::cout << (alg::rotation(0.1F) * shape) << std::endl;
}