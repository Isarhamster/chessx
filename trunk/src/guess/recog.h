//////////////////////////////////////////////////////////////////////
//
//  FILE:       recog.h
//              Endgame knowledge recognition class
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.4
//
//  Notice:     Copyright (c) 2002 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#include "guessengine.h"
#include "position.h"

namespace Guess
{

// The Recognizer class provides score bound information for chess
// endgames.

// Only positions with a total of 6 or fewer pieces (including kings
// and pawns) are potentially recognizable.
static const unsigned int MAX_RECOGNIZED_PIECES = 6;

// Recognition value conversion: a recognition score contains a
//   regular score shifted left 4 bits to make room for a score flag.

inline int recogValue(scoreFlagT flag, int score)
{
    return ((score * 16) | flag);
}
inline int recogScore(int value)
{
    return value / 16;
}
inline scoreFlagT recogFlag(int value)
{
    return value & 15;
}

class Recognizer
{
private:
    static int   KPK(Position * pos);

    static int   KBBK(Position * pos);
    static int   KBNK(Position * pos);
    static int   KBPK(Position * pos);

    static int   KRKB(Position * pos);
    static int   KRKN(Position * pos);
    static int   KMKP(Position * pos);
    static int   KQKP(Position * pos);
    static int   KRKP(Position * pos);

    static int   KRPKR(Position * pos);

public:
    static unsigned int  MaxPieces(void)
    {
        return MAX_RECOGNIZED_PIECES;
    }
    static int   Recognize(Position * pos);
};

} // End namespace Guess

//////////////////////////////////////////////////////////////////////
//  EOF: recog.h
//////////////////////////////////////////////////////////////////////
