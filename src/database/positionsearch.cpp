/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "positionsearch.h"

#include "game.h"
#include "database.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/* PositionSearch Class
 * ******************************/
PositionSearch::PositionSearch()
{
}

PositionSearch::PositionSearch(Database* db, const Board& position):Search(db)
{
    setPosition(position);
}

void PositionSearch::setPosition(const Board& position)
{
    m_position = position;
}

int PositionSearch::matches(GameId index) const
{
    Game g;
    m_database->loadGameMoves(index, g);
    return (g.findPosition(m_position) != NO_MOVE);
}

