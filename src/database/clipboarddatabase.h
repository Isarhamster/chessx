/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef CLIPBOARDDATABASE_H
#define CLIPBOARDDATABASE_H

#include "memorydatabase.h"

class ClipboardDatabase : public MemoryDatabase
{
public:
    ClipboardDatabase();

    virtual QString name() const;
    virtual QString filename() const;
};

#endif // CLIPBOARDDATABASE_H
