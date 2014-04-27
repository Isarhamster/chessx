/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "movedata.h"

void MoveData::addGame(Game& g, Color c, MoveType movetype)
{
    if(!count)
        san = (movetype == StandardMove) ? g.moveToSan(Game::MoveOnly, Game::PreviousMove)
               : qApp->translate("MoveData", "[end]");
    ++count;
    result[g.result()]++;
    unsigned elo = (c == White) ? g.tag("WhiteElo").toInt() : g.tag("BlackElo").toInt();
    if(elo >= 1000)
    {
        rating += elo;
        ++rated;
    }
    unsigned y = g.tag("Date").section(".", 0, 0).toInt();
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
