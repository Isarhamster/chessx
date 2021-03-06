/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef STREAMDATABASE_H
#define STREAMDATABASE_H

#include <QObject>
#include "pgndatabase.h"

class StreamDatabase : public PgnDatabase
{
public:
    bool loadNextGame(GameX &game);

protected:
    virtual bool hasIndexFile() const { return false; }
    virtual bool parseFile();
};

#endif // STREAMDATABASE_H
