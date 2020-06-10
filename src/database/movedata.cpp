/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "movedata.h"

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

double ResultsCounter::scorePercentage() const
{
    // count unknown as draws
    auto score = 1.0 * m_counts[WhiteWin] + 0.5 * (m_counts[Draw] + m_counts[ResultUnknown]);
    auto total = count();
    return total? (score / static_cast<double>(total) * 100.0): 0.0;
}

double ResultsCounter::whiteWinPercentage() const
{
    auto count = m_counts[WhiteWin];
    auto total = m_counts[WhiteWin] + m_counts[Draw] + m_counts[BlackWin];
    return total? (static_cast<double>(count) / static_cast<double>(total) * 100.0): 0.0;
}

double ResultsCounter::blackWinPercentage() const
{
    auto count = m_counts[BlackWin];
    auto total = m_counts[WhiteWin] + m_counts[Draw] + m_counts[BlackWin];
    return total? (static_cast<double>(count) / static_cast<double>(total) * 100.0): 0.0;
}

bool operator<(const MoveData& m1, const MoveData& m2)
{
    auto c1 = m1.results.count();
    auto c2 = m2.results.count();
    return c1 < c2 || (c1 == c2 && m1.san < m2.san);
}

bool operator>(const MoveData& m1, const MoveData& m2)
{
    auto c1 = m1.results.count();
    auto c2 = m2.results.count();
    return c1 > c2 || (c1 == c2 && m1.san > m2.san);
}

bool compareMove(const MoveData& m1, const MoveData& m2)
{
    return m1.san < m2.san;
}

bool compareScore(const MoveData& m1, const MoveData& m2)
{
    auto s1 = m1.results.scorePercentage();
    auto s2 = m2.results.scorePercentage();
    return s1 < s2 || (s1 == s2 && m1.san < m2.san);
}

bool compareRating(const MoveData& m1, const MoveData& m2)
{
    auto r1 = m1.rating.average();
    auto r2 = m2.rating.average();
    return r1 < r2 || (r1 == r2 && m1.san < m2.san);
}

bool compareYear(const MoveData& m1, const MoveData& m2)
{
    auto y1 = m1.year.average();
    auto y2 = m2.year.average();
    return y1 < y2 || (y1 == y2 && m1.san < m2.san);
}
