///////////////////////////////////////////////////////////////////////////
//
//  FILE:       error.h
//              Error codes
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    1.6
//
//  Notice:     Copyright (c) 1999  Shane Hudson. all rights reserved.
//              Copyright (c) 2014  Benini Fulvio. all rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
///////////////////////////////////////////////////////////////////////////


#ifndef SCID_ERROR_H
#define SCID_ERROR_H

typedef   unsigned short   errorT;

const errorT
    OK = 0,

    // General Error:
    ERROR               =   1,
    ERROR_UserCancel    =   2,
    ERROR_BadArg        =   3,
    
    // File I/O
    ERROR_FileOpen       = 101,
    ERROR_FileWrite      = 102,
    ERROR_FileRead       = 103,
    ERROR_FileSeek       = 104,
    ERROR_BadMagic       = 105,
    ERROR_FileNotOpen    = 106,
    ERROR_FileInUse      = 107,
    ERROR_FileMode       = 108,
    ERROR_FileVersion    = 109,
    ERROR_OldScidVersion = 110,
    ERROR_FileReadOnly   = 111,
    ERROR_CompactRemove  = 121,

    // Memory Allocation, corrupt data
    ERROR_MallocFailed   = 151,
    ERROR_CorruptData    = 152,   ERROR_Corrupt = 152,

    // Database errors
    ERROR_Full = 201,
    ERROR_NameNotFound   = 202,  ERROR_NotFound = 202,
    ERROR_NameExists     = 203,  ERROR_Exists = 203,
    ERROR_NameBaseEmpty  = 204,  ERROR_Empty = 204,
    ERROR_NoMatchFound   = 205,
    ERROR_NameDataLoss   = 206,
    ERROR_NameTooLong    = 207,
    ERROR_NameLimit      = 208,
    ERROR_OffsetLimit    = 209,
    ERROR_GameLengthLimit= 210,
    ERROR_NumGamesLimit  = 211,

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
    ERROR_CastlingAvailability = 408,

    // Buffer errors
    ERROR_BufferFull = 601,
    ERROR_BufferRead = 602,

    // Codec errors
    ERROR_CodecUnsupFeat = 701;


static_assert(OK == false);

#endif   // #ifdef SCID_ERROR_H

///////////////////////////////////////////////////////////////////////////
//  EOF:    error.h
///////////////////////////////////////////////////////////////////////////

