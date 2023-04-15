#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "movedata.h"

constexpr int MinValue = -100;
constexpr int MaxValue = 100;

using Metrics = IntegralMetrics<MinValue, MaxValue>;

TEST_CASE("IntegralMetrics computes aggregates")
{
    SECTION("empty")
    {
        Metrics m {};
        CHECK(m.count() == 0);
        CHECK(m.average() == 0);
    }
    SECTION("all valid")
    {
        Metrics m {10, 20, 30};
        CHECK(m.count() == 3);
        CHECK(m.average() == 20);
    }
    SECTION("mixed")
    {
        Metrics m {10, 1000, 20, 30, -1000};
        CHECK(m.count() == 3);
        CHECK(m.average() == 20);
    }
    SECTION("update()")
    {
        Metrics m;
        m.update(10, 1000);
        m.update(0, 1000);

        CHECK(m.count() == 2000);
        CHECK(m.average() == 5);
    }
}

TEST_CASE("testing IntegralMetrics bounds checking")
{
    Metrics m1 { MinValue, MaxValue };
    CHECK(m1.count() == 2);

    Metrics m2 { MinValue - 1, MaxValue + 1};
    CHECK(m2.count() == 0);
}
