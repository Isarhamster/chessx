///////////////////////////////////////////////////////////////////////////
//
//  FILE:       tokens.h
//              Tokens used for scanning PGN.
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.4
//
//  Notice:     Copyright (c) 1999-2001  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
///////////////////////////////////////////////////////////////////////////


#ifndef SCID_TOKENS_H
#define SCID_TOKENS_H

namespace Guess
{

//
// TOKENS used by PGN scanner
//

typedef unsigned int tokenT;

#define MAX_TOKEN 58;

const tokenT
TOKEN_EndOfInput = 0,  // No more characters to read from input source
TOKEN_Invalid    = 1,  // Unknown token
TOKEN_MoveNum    = 3,  // Move Number. e.g.  "1." or "2..."
TOKEN_Ignore     = 4;  // Anything ignored, like "e.p."

const tokenT
TOKEN_Move_Pawn         = 5,  // Excludes Promotions
TOKEN_Move_Promote      = 6,  // Promotions
TOKEN_Move_Piece        = 7,  // Includes ambiguous moves and captures
TOKEN_Move_Castle_King  = 8,
TOKEN_Move_Castle_Queen = 9,
TOKEN_Move_Null         = 10;

inline bool
TOKEN_isMove(tokenT token)
{
    return (token >= TOKEN_Move_Pawn  &&  token <= TOKEN_Move_Null);
}

inline bool
TOKEN_isPawnMove(tokenT token)
{
    return (token == TOKEN_Move_Pawn  ||  token == TOKEN_Move_Promote);
}

// Tags: currently only TOKEN_Tag. Maybe eventually have TOKEN_Tag_Event, etc.
const tokenT
TOKEN_Tag = 20,
TOKEN_TagEnd = 21;
#define TOKEN_isTag(x)  ((x) == TOKEN_Tag)

// Results
const tokenT
TOKEN_Result_White = 41,    // White wins
TOKEN_Result_Black = 42,    // Black wins
TOKEN_Result_Draw  = 43,    // Draw
TOKEN_Result_Star  = 44;    // Incomplete/unknown ("*")
#define TOKEN_isResult(x)  (((x) >= 41)  &&  ((x) <= 44))

// Comments, Variations
const tokenT
TOKEN_Nag =         51,  // $xxx   example: $12
TOKEN_Suffix =      52,  // !, ?, !!, ??, !?, ?!
TOKEN_Check =       53,  // "+"
TOKEN_Mate =        54,  // "#"
TOKEN_Comment =     55,  // { .... }
TOKEN_LineComment = 56,  // "%" or ";" until end of line
TOKEN_VarStart =    57,  // "(" ....
TOKEN_VarEnd =      58,  //  .... ")"
TOKEN_CommentEnd =  59;  // "}" outside of comment, should not happen

} // End namespace Guess

#endif  // SCID_TOKENS_H

///////////////////////////////////////////////////////////////////////////
//  EOF: tokens.h
///////////////////////////////////////////////////////////////////////////
