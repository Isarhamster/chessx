/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef FICSDATABASE_H
#define FICSDATABASE_H

#include "memorydatabase.h"
#include "game.h"

class FicsDatabase : public MemoryDatabase
{
    Q_OBJECT

public:
    FicsDatabase();

    virtual QString name() const;
};

#endif // FICSDATABASE_H
