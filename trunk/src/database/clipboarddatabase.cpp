/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "clipboarddatabase.h"

ClipboardDatabase::ClipboardDatabase()
{
}

QString ClipboardDatabase::name() const
{
    return tr("Clipboard");
}

QString ClipboardDatabase::filename() const
{
    return "Clipboard";
}
