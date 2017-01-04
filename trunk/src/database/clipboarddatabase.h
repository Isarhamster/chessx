/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef CLIPBOARDDATABASE_H
#define CLIPBOARDDATABASE_H

#include "memorydatabase.h"

class ClipboardDatabase : public MemoryDatabase
{
    Q_OBJECT

public:
    ClipboardDatabase();

    virtual QString name() const;
    virtual QString filename() const;

    virtual bool IsClipboard() const { return true; }

    static QString displayName();
};

#endif // CLIPBOARDDATABASE_H
