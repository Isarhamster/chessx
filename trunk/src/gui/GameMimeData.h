/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef GAMEMIMEDATA_H
#define GAMEMIMEDATA_H

#include <QMimeData>
#include <QModelIndex>
#include "game.h"

class GameMimeData : public QMimeData
{
    Q_OBJECT

public:
    QList<GameId> m_indexList;
    QString source;
};

class DbMimeData : public QMimeData
{
    Q_OBJECT
};

#endif // GAMEMIMEDATA_H
