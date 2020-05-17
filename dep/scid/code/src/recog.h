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

#include "engine.h"
#include "position.h"

// Recognition value conversion: a recognition score contains a
//   regular score shifted left 4 bits to make room for a score flag.

inline int recogValue (scoreFlagT flag, int score) {
    return ((score * 16) | flag);
}
inline int recogScore (int value) { return value / 16; }
inline scoreFlagT recogFlag (int value) { return value & 15; }

// The Recognizer class provides score bound information for chess endgames.
namespace Recognizer {

constexpr uint MaxPieces() {
	// Only positions with a total of 6 or fewer pieces (including kings
	// and pawns) are potentially recognizable.
	return 6;
}
int Recognize(Position* pos);

} // namespace Recognizer

//////////////////////////////////////////////////////////////////////
//  EOF: recog.h
//////////////////////////////////////////////////////////////////////
