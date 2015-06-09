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
    QList<int> m_indexList;
};

class DbMimeData : public QMimeData
{
    Q_OBJECT
};

#endif // GAMEMIMEDATA_H
