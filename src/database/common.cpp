/***************************************************************************
 *   (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>           *
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "common.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

Piece& operator++(Piece& w)
{
    if(w < BlackPawn)
    {
        return w = Piece(w + 1);
    }
    else
    {
        return w = Empty;
    }
}

Piece operator++(Piece& w, int)
{
    Piece alt = w;
    ++w;
    return alt;
}

QString resultString(Result result)
{
    switch(result)
    {
    case WhiteWin:
        return "1-0";
    case Draw:
        return "1/2-1/2";
    case BlackWin:
        return "0-1";
    case ResultUnknown:
    default:
        return "*";
    }
}

bool isStandardTag(QString tag)
{
    for(int i = 0; i < 7; ++i)
    {
        if (tag.compare(StandardTags[i])==0)
        {
            return true;
        }
    }
    return false;
}


Result ResultFromString(QString res)
{
    if(res.startsWith("1/2"))
    {
        return Draw;
    }
    else if(res.startsWith('1'))
    {
        return WhiteWin;
    }
    else if(res.startsWith('0'))
    {
        return BlackWin;
    }
    else
    {
        return ResultUnknown;
    }
}
