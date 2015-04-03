/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "clipboarddatabase.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
