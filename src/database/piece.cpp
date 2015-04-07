/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "piece.h"

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


