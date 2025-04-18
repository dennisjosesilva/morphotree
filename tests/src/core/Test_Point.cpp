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
    SECTION("We can multiply two points") {
        mt::Point<TestType> r = p * q;
        REQUIRE(r == mt::Point<TestType>{35, 20});

        r = p.mult(q);
        REQUIRE(r == mt::Point<TestType>{35, 20});
    }
    SECTION("We can divide two points") {
        mt::Point<TestType> r = p / q;
        TestType rx = p.x() / q.x();
        TestType ry = p.y() / q.y();

        REQUIRE(r == mt::Point<TestType>{rx, ry});
    }
    SECTION("We can add two points in place") {
        p += q;
        REQUIRE(p == mt::Point<TestType>{12, 9});
    }
    SECTION("We can subtract two points in place") {
        p -= q;
        REQUIRE(p == mt::Point<TestType>{2, 1});
    }
    SECTION("We can multiply two points in place") {
        p *= q;
        REQUIRE(p == mt::Point<TestType>{35, 20});
    }
    SECTION("We can divide two points in place") {
        TestType rx = p.x() / q.x();
        TestType ry = p.y() / q.y();
        p /= q;

        REQUIRE(p == mt::Point<TestType>{rx, ry});
    }
 }