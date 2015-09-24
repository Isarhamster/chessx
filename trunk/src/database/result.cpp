/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "result.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
    else if(res.endsWith("1/2"))
    {
        return Draw;
    }
    else if(res.endsWith("0-1"))
    {
        return BlackWin;
    }
    else if(res.endsWith("1-0"))
    {
        return WhiteWin;
    }
    else
    {
        return ResultUnknown;
    }
}
