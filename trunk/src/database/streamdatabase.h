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
    StreamDatabase();

    bool loadNextGame(Game &game);

protected:
    virtual bool hasIndexFile() const { return false; }
    virtual bool parseFile();
};

#endif // STREAMDATABASE_H
