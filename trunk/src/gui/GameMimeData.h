/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef GAMEMIMEDATA_H
#define GAMEMIMEDATA_H

#include <QMimeData>
#include "game.h"

class GameMimeData : public QMimeData
{
    Q_OBJECT

public:
    Game m_game;
};

#endif // GAMEMIMEDATA_H
