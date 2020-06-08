#include <vector>

#include "doctest.h"

#include "movedata.h"

TEST_CASE("testing ResultsCounter")
{
    const ResultsCounter unit;

    SUBCASE("equality")
    {
        ResultsCounter c1;
        CHECK_EQ(c1 == unit, true);
        c1.update(WhiteWin);
        CHECK_EQ(c1 == unit, false);

        ResultsCounter c2 { WhiteWin };
        CHECK_EQ(c2 == c1, true);

        std::vector<ResultsCounter> counters {
            {},
            {ResultUnknown},
            {WhiteWin},
            {Draw},
            {BlackWin},
        };

        for (const auto& cntr: counters)
        {
            CHECK_EQ(cntr == cntr, true);
        }

        for (size_t i = 0; i < counters.size(); ++i)
        {
            for (auto j = i + 1; j < counters.size(); ++j)
            {
                CHECK_EQ(counters[i] == counters[j], false);
                CHECK_EQ(counters[j] == counters[i], false);
            }
        }
    }

    SUBCASE("count()")
    {
        SUBCASE("unit")
        {
            CHECK_EQ(unit.count(), 0);
            CHECK_EQ(unit.count(ResultUnknown), 0);
            CHECK_EQ(unit.count(WhiteWin), 0);
            CHECK_EQ(unit.count(Draw), 0);
            CHECK_EQ(unit.count(BlackWin), 0);
        }
        SUBCASE("+")
        {
            ResultsCounter c { WhiteWin };
            CHECK_EQ(c.count(), 1);
            CHECK_EQ(c.count(ResultUnknown), 0);
            CHECK_EQ(c.count(WhiteWin), 1);
            CHECK_EQ(c.count(Draw), 0);
            CHECK_EQ(c.count(BlackWin), 0);
        }
        SUBCASE("=")
        {
            ResultsCounter c { Draw };
            CHECK_EQ(c.count(), 1);
            CHECK_EQ(c.count(ResultUnknown), 0);
            CHECK_EQ(c.count(WhiteWin), 0);
            CHECK_EQ(c.count(Draw), 1);
            CHECK_EQ(c.count(BlackWin), 0);
        }
        SUBCASE("-")
        {
            ResultsCounter c { BlackWin };
            CHECK_EQ(c.count(), 1);
            CHECK_EQ(c.count(ResultUnknown), 0);
            CHECK_EQ(c.count(WhiteWin), 0);
            CHECK_EQ(c.count(Draw), 0);
            CHECK_EQ(c.count(BlackWin), 1);
        }
        SUBCASE("*")
        {
            ResultsCounter c { ResultUnknown };
            CHECK_EQ(c.count(), 1);
            CHECK_EQ(c.count(ResultUnknown), 1);
            CHECK_EQ(c.count(WhiteWin), 0);
            CHECK_EQ(c.count(Draw), 0);
            CHECK_EQ(c.count(BlackWin), 0);
        }
        SUBCASE("mixed")
        {
            ResultsCounter c { WhiteWin, Draw, BlackWin, Draw, ResultUnknown};
            CHECK_EQ(c.count(), 5);
            CHECK_EQ(c.count(ResultUnknown), 1);
            CHECK_EQ(c.count(WhiteWin), 1);
            CHECK_EQ(c.count(Draw), 2);
            CHECK_EQ(c.count(BlackWin), 1);
        }
    }

    SUBCASE("scorePercentage()")
    {
        SUBCASE("unit")
        {
            CHECK_EQ(unit.scorePercentage(), 0.0);
        }
        SUBCASE("+")
        {
            ResultsCounter c { WhiteWin };
            CHECK_EQ(c.scorePercentage(), 100.0);
        }
        SUBCASE("=")
        {
            ResultsCounter c { Draw };
            CHECK_EQ(c.scorePercentage(), 50.0);
        }
        SUBCASE("*")
        {
            ResultsCounter c { ResultUnknown };
            CHECK_EQ(c.scorePercentage(), 50.0);
        }
        SUBCASE("-")
        {
            ResultsCounter c { BlackWin };
            CHECK_EQ(c.scorePercentage(), 0.0);
        }
        SUBCASE("mixed")
        {
            ResultsCounter c { WhiteWin, Draw, ResultUnknown };
            CHECK_EQ(c.scorePercentage(), doctest::Approx(67).epsilon(0.01));
        }
    }

    SUBCASE("whiteWinPercentage()")
    {
        SUBCASE("unit")
        {
            CHECK_EQ(unit.whiteWinPercentage(), 0.0);
        }
        SUBCASE("+")
        {
            ResultsCounter c { WhiteWin };
            CHECK_EQ(c.whiteWinPercentage(), 100.0);
        }
        SUBCASE("=")
        {
            ResultsCounter c { Draw };
            CHECK_EQ(c.whiteWinPercentage(), 0.0);
        }
        SUBCASE("*")
        {
            ResultsCounter c { ResultUnknown };
            CHECK_EQ(c.whiteWinPercentage(), 0.0);
        }
        SUBCASE("-")
        {
            ResultsCounter c { BlackWin };
            CHECK_EQ(c.whiteWinPercentage(), 0.0);
        }
        SUBCASE("mixed")
        {
            ResultsCounter c { WhiteWin, Draw, ResultUnknown, BlackWin, WhiteWin };
            CHECK_EQ(c.whiteWinPercentage(), 50.0);
        }
    }

    SUBCASE("blackWinPercentage()")
    {
        SUBCASE("unit")
        {
            CHECK_EQ(unit.blackWinPercentage(), 0.0);
        }
        SUBCASE("+")
        {
            ResultsCounter c { WhiteWin };
            CHECK_EQ(c.blackWinPercentage(), 0.0);
        }
        SUBCASE("=")
        {
            ResultsCounter c { Draw };
            CHECK_EQ(c.blackWinPercentage(), 0.0);
        }
        SUBCASE("*")
        {
            ResultsCounter c { ResultUnknown };
            CHECK_EQ(c.blackWinPercentage(), 0.0);
        }
        SUBCASE("-")
        {
            ResultsCounter c { BlackWin };
            CHECK_EQ(c.blackWinPercentage(), 100.0);
        }
        SUBCASE("mixed")
        {
            ResultsCounter c { WhiteWin, Draw, ResultUnknown, BlackWin, WhiteWin };
            CHECK_EQ(c.blackWinPercentage(), 25.0);
        }
    }
}

