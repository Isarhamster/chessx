/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef POSITIONSEARCH_H
#define POSITIONSEARCH_H

#include "search.h"
#include "board.h"

/** @ingroup Search
The PositionSearch class is a search that checks for given position.
@todo Performance is seriously bad
*/
class PositionSearch : public Search
{
    Q_OBJECT

public:
    /** Empty constructor. */
    PositionSearch();
    /** Standard constructor. */
    PositionSearch(Database* db, const BoardX& position);
    /** Sets sought position. */
    void setPosition(const BoardX & position);
    /** Return moveId the move of  after which the game matches the search + 1. E.g. for standard game and chess start position
        1 is returned.
    */
    virtual int matches(GameId index) const;
private:
    BoardX m_position;
};

#endif // POSITIONSEARCH_H
