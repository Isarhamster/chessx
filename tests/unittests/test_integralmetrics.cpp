#include <vector>

#include "doctest.h"

#include "movedata.h"

constexpr int MinValue = -100;
constexpr int MaxValue = 100;

using Metrics = IntegralMetrics<MinValue, MaxValue>;

TEST_CASE("testing IntegralMetrics")
{
    SUBCASE("empty")
    {
        Metrics m {};
        CHECK_EQ(m.count(), 0);
        CHECK_EQ(m.average(), 0);
    }
    SUBCASE("all valid")
    {
        Metrics m {10, 20, 30};
        CHECK_EQ(m.count(), 3);
        CHECK_EQ(m.average(), 20);
    }
    SUBCASE("mixed")
    {
        Metrics m {10, 1000, 20, 30, -1000};
        CHECK_EQ(m.count(), 3);
        CHECK_EQ(m.average(), 20);
    }
    SUBCASE("update()")
    {
        Metrics m;
        m.update(10, 1000);
        m.update(0, 1000);

        CHECK_EQ(m.count(), 2000);
        CHECK_EQ(m.average(), 5);
    }
}

TEST_CASE("testing IntegralMetrics bounds checking")
{
    Metrics m1 { MinValue, MaxValue };
    CHECK_EQ(m1.count(), 2);

    Metrics m2 { MinValue - 1, MaxValue + 1};
    CHECK_EQ(m2.count(), 0);
}
