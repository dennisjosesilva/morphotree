#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <morphotree/core/point.hpp>

namespace mt = morphotree;

TEMPLATE_TEST_CASE("Points can be perform basic math opetarors", "[Point][template]", float, double, 
    mt::uint8, mt::uint16, mt::uint32, mt::int8, mt::int16, mt::int32)
{
    mt::Point<TestType> p{ 7, 5};
    mt::Point<TestType> q{ 5, 4};
    
    SECTION("We can add two points") {
        mt::Point<TestType> r = p + q;
        REQUIRE(r == mt::Point<TestType>{12, 9});

        r = p.add(q);
        REQUIRE(r == mt::Point<TestType>{12, 9});
    }
    SECTION("We can subtract two points") {
        mt::Point<TestType> r = p - q;
        REQUIRE(r == mt::Point<TestType>{2, 1});
        
        r = p.sub(q);
        REQUIRE(r == mt::Point<TestType>{2, 1});
    }
}
