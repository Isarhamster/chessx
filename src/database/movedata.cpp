/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "movedata.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

void MoveData::addGame(Game& g, Color c, MoveType movetype)
{
    if(!count)
    {
        san = (movetype == StandardMove) ? g.moveToSan(Game::MoveOnly, Game::PreviousMove)
               : qApp->translate("MoveData", "[end]");
        localsan = (movetype == StandardMove) ? g.moveToSan(Game::TranslatePiece, Game::PreviousMove)
               : qApp->translate("MoveData", "[end]");
    }
    ++count;
    result[g.result()]++;
    unsigned int elo = (c == White) ? g.tag("WhiteElo").toUInt() : g.tag("BlackElo").toUInt();
    if(elo >= 1000)
    {
        rating += elo;
        ++rated;
    }
    unsigned int y = g.tag("Date").section(".", 0, 0).toUInt();
    if(y > 1000)
    {
        year += y;
        ++dated;
    }
}

MoveData::MoveData()
{
    count = 0;
    for(int  r = ResultUnknown; r <= BlackWin; ++r)
    {
        result[r] = 0;
    }
    year = rating = 0;
    dated = rated = 0;
}

double MoveData::percentage() const
{
    unsigned c = result[ResultUnknown] + 2 * result[WhiteWin] + result[Draw];
    return count ? c * 500 / count / 10.0 : 0;
}

bool MoveData::hasPercent() const
{
    int n = 0;
    for (int i=0; i<4; ++i)
    {
        n += result[i];
    }
    return (n>0);
}

int MoveData::averageRating() const
{
    return rated ? (rating / (unsigned long long) rated) : 0;
}

int MoveData::averageYear() const
{
    return dated ? year / dated : 0;
}

bool operator<(const MoveData& m1, const MoveData& m2)
{
    return m1.count < m2.count || (m1.count == m2.count && m1.san < m2.san);
}

bool operator>(const MoveData& m1, const MoveData& m2)
{
    return m1.count > m2.count || (m1.count == m2.count && m1.san > m2.san);
}

bool compareMove(const MoveData& m1, const MoveData& m2)
{
    return m1.san < m2.san;
}

bool compareScore(const MoveData& m1, const MoveData& m2)
{
    return m1.percentage() < m2.percentage() ||
           (m1.percentage() == m2.percentage() && m1.san < m2.san);
}

bool compareRating(const MoveData& m1, const MoveData& m2)
{
    return m1.averageRating() < m2.averageRating() ||
           (m1.averageRating() == m2.averageRating() && m1.san < m2.san);
}

bool compareYear(const MoveData& m1, const MoveData& m2)
{
    return m1.averageYear() < m2.averageYear() ||
           (m1.averageYear() == m2.averageYear() && m1.san < m2.san);
}
