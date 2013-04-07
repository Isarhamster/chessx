///////////////////////////////////////////////////////////////////////////
//
//  FILE:       error.h
//              Error codes
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    1.6
//
//  Notice:     Copyright (c) 1999  Shane Hudson. all rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
///////////////////////////////////////////////////////////////////////////


#ifndef SCID_ERROR_H
#define SCID_ERROR_H

namespace Guess
{

typedef   unsigned short   errorT;

const errorT
    OK = 0,
    
    // File I/O
    ERROR_FileOpen      = 101,
    ERROR_FileWrite     = 102,
    ERROR_FileRead      = 103,
    ERROR_FileSeek      = 104,
    ERROR_BadMagic      = 105,
    ERROR_FileNotOpen   = 106,
    ERROR_FileInUse     = 107,
    ERROR_FileMode      = 108,
    ERROR_FileVersion   = 109,
    ERROR_OldScidVersion = 110,

    // Memory Allocation, corrupt data
    ERROR_MallocFailed   = 151,
    ERROR_CorruptData    = 152,   ERROR_Corrupt = 152,

    // NameBase, Index errors
    ERROR_NameBaseFull   = 201,  ERROR_Full = 201,
    ERROR_NameNotFound   = 202,  ERROR_NotFound = 202,
    ERROR_NameExists     = 203,  ERROR_Exists = 203,
    ERROR_NameBaseEmpty  = 204,  ERROR_Empty = 204,
    ERROR_NoMatchFound   = 205,

    ERROR_IndexFull      = 251,

    // Position errors
    ERROR_InvalidFEN     = 301,
    ERROR_InvalidMove    = 302,
    ERROR_PieceCount     = 303,

    // Game Errors
    ERROR_Game = 400,
    ERROR_EndOfMoveList  = 401,
    ERROR_StartOfMoveList = 402,
    ERROR_NoVariation = 403,
    ERROR_EmptyVariation = 404,
    ERROR_VariationLimit = 405,
    ERROR_Decode = 406,
    ERROR_GameFull = 407,

    // Scan Errors
    ERROR_PGNTag = 501,

    // Buffer errors
    ERROR_BufferFull = 601,
    ERROR_BufferRead = 602;

} // End namespace Guess

#endif   // #ifdef SCID_ERROR_H

///////////////////////////////////////////////////////////////////////////
//  EOF:    error.h
///////////////////////////////////////////////////////////////////////////

