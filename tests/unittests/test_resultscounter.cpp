#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "movedata.h"

TEST_CASE("testing ResultsCounter")
{
    const ResultsCounter unit;

    SECTION("equality")
    {
        ResultsCounter c1;
        CHECK(c1 == unit);
        c1.update(WhiteWin);
        CHECK_FALSE(c1 == unit);

        ResultsCounter c2 { WhiteWin };
        CHECK(c2 == c1);

        std::vector<ResultsCounter> counters {
            {},
            {ResultUnknown},
            {WhiteWin},
            {Draw},
            {BlackWin},
        };

        for (const auto& cntr: counters)
        {
            CHECK(cntr == cntr);
        }

        for (size_t i = 0; i < counters.size(); ++i)
        {
            for (auto j = i + 1; j < counters.size(); ++j)
            {
                CHECK_FALSE(counters[i] == counters[j]);
                CHECK_FALSE(counters[j] == counters[i]);
            }
        }
    }

    SECTION("count()")
    {
        SECTION("unit")
        {
            CHECK(unit.count() == 0);
            CHECK(unit.count(ResultUnknown) == 0);
            CHECK(unit.count(WhiteWin) == 0);
            CHECK(unit.count(Draw) == 0);
            CHECK(unit.count(BlackWin) == 0);
        }
        SECTION("+")
        {
            ResultsCounter c { WhiteWin };
            CHECK(c.count() == 1);
            CHECK(c.count(ResultUnknown) == 0);
            CHECK(c.count(WhiteWin) == 1);
            CHECK(c.count(Draw) == 0);
            CHECK(c.count(BlackWin) == 0);
        }
        SECTION("=")
        {
            ResultsCounter c { Draw };
            CHECK(c.count() == 1);
            CHECK(c.count(ResultUnknown) == 0);
            CHECK(c.count(WhiteWin) == 0);
            CHECK(c.count(Draw) == 1);
            CHECK(c.count(BlackWin) == 0);
        }
        SECTION("-")
        {
            ResultsCounter c { BlackWin };
            CHECK(c.count() == 1);
            CHECK(c.count(ResultUnknown) == 0);
            CHECK(c.count(WhiteWin) == 0);
            CHECK(c.count(Draw) == 0);
            CHECK(c.count(BlackWin) == 1);
        }
        SECTION("*")
        {
            ResultsCounter c { ResultUnknown };
            CHECK(c.count() == 1);
            CHECK(c.count(ResultUnknown) == 1);
            CHECK(c.count(WhiteWin) == 0);
            CHECK(c.count(Draw) == 0);
            CHECK(c.count(BlackWin) == 0);
        }
        SECTION("mixed")
        {
            ResultsCounter c { WhiteWin, Draw, BlackWin, Draw, ResultUnknown};
            CHECK(c.count() == 5);
            CHECK(c.count(ResultUnknown) == 1);
            CHECK(c.count(WhiteWin) == 1);
            CHECK(c.count(Draw) == 2);
            CHECK(c.count(BlackWin) == 1);
        }
    }

    SECTION("scorePercentage()")
    {
        SECTION("unit")
        {
            REQUIRE(unit.scorePercentage() == 0.0);
        }
        SECTION("+")
        {
            ResultsCounter c { WhiteWin };
            REQUIRE(c.scorePercentage() == 100.0);
        }
        SECTION("=")
        {
            ResultsCounter c { Draw };
            REQUIRE(c.scorePercentage() == 50.0);
        }
        SECTION("*")
        {
            ResultsCounter c { ResultUnknown };
            REQUIRE(c.scorePercentage() == 50.0);
        }
        SECTION("-")
        {
            ResultsCounter c { BlackWin };
            REQUIRE(c.scorePercentage() == 0.0);
        }
        SECTION("mixed")
        {
            ResultsCounter c { WhiteWin, Draw, ResultUnknown };
            auto expected = 66.67;
            REQUIRE_THAT(c.scorePercentage(), Catch::Matchers::WithinAbs(expected, 0.01));
        }
    }

    SECTION("whiteWinPercentage()")
    {
        SECTION("unit")
        {
            REQUIRE(unit.whiteWinPercentage() == 0.0);
        }
        SECTION("+")
        {
            ResultsCounter c { WhiteWin };
            REQUIRE(c.whiteWinPercentage() == 100.0);
        }
        SECTION("=")
        {
            ResultsCounter c { Draw };
            REQUIRE(c.whiteWinPercentage() == 0.0);
        }
        SECTION("*")
        {
            ResultsCounter c { ResultUnknown };
            REQUIRE(c.whiteWinPercentage() == 0.0);
        }
        SECTION("-")
        {
            ResultsCounter c { BlackWin };
            REQUIRE(c.whiteWinPercentage() == 0.0);
        }
        SECTION("mixed")
        {
            ResultsCounter c { WhiteWin, Draw, ResultUnknown, BlackWin, WhiteWin };
            REQUIRE(c.whiteWinPercentage() == 50.0);
        }
    }

    SECTION("blackWinPercentage()")
    {
        SECTION("unit")
        {
            REQUIRE(unit.blackWinPercentage() == 0.0);
        }
        SECTION("+")
        {
            ResultsCounter c { WhiteWin };
            REQUIRE(c.blackWinPercentage() == 0.0);
        }
        SECTION("=")
        {
            ResultsCounter c { Draw };
            REQUIRE(c.blackWinPercentage() == 0.0);
        }
        SECTION("*")
        {
            ResultsCounter c { ResultUnknown };
            REQUIRE(c.blackWinPercentage() == 0.0);
        }
        SECTION("-")
        {
            ResultsCounter c { BlackWin };
            REQUIRE(c.blackWinPercentage() == 100.0);
        }
        SECTION("mixed")
        {
            ResultsCounter c { WhiteWin, Draw, ResultUnknown, BlackWin, WhiteWin };
            REQUIRE(c.blackWinPercentage() == 25.0);
        }
    }

    SECTION("update()")
    {
        // check `count` parameter
        ResultsCounter c;
        c.update(ResultUnknown, 10);
        c.update(WhiteWin, 20);
        c.update(Draw, 30);
        c.update(BlackWin, 40);

        REQUIRE(c.count(ResultUnknown) == 10);
        REQUIRE(c.count(WhiteWin) == 20);
        REQUIRE(c.count(Draw) == 30);
        REQUIRE(c.count(BlackWin) == 40);
        REQUIRE(c.count() == 100);
    }
}

