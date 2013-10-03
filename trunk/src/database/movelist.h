/***************************************************************************
 *   (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>           *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

/** @ingroup Core
   Low level list of moves, used by bitboard
*/

#ifndef __MOVELIST_H__
#define __MOVELIST_H__

#include "move.h"
#include <QVector>

class MoveList : public QVector<Move>
{
public:
    MoveList() : QVector<Move>() {}
    inline Move& add()
    {
        push_back(Move());
        return back();
    }
};

#endif // __MOVELIST_H__
