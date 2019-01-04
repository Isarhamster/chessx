/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef GAMEID_H
#define GAMEID_H

#include <QtCore>

typedef quint32 GameId;
static const GameId InvalidGameId = static_cast<GameId>(-1);
#define VALID_INDEX(index) (index!=InvalidGameId)

#endif // GAMEID_H

