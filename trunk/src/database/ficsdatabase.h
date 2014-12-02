/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef FICSDATABASE_H
#define FICSDATABASE_H

#include "memorydatabase.h"
#include "game.h"

class FicsDatabase : public MemoryDatabase
{
public:
    FicsDatabase();

    virtual QString name() const;
    virtual QString filename() const;
};

#endif // FICSDATABASE_H
