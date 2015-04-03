/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "ficsdatabase.h"
#include "game.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

FicsDatabase::FicsDatabase()
{
}

QString FicsDatabase::name() const
{
    return "FICS";
}

QString FicsDatabase::filename() const
{
    return "FICS";
}
