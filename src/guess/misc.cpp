//////////////////////////////////////////////////////////////////////
//
//  FILE:       misc.cpp
//              Miscellaneous routines (File I/O, etc)
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2001-2003  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
#include "misc.h"
#include "error.h"
#include "misc.h"

#include <QtCore>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>     // For isspace() function.
#include <sys/stat.h>  // Needed for fileSize() function.

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

namespace Guess
{


// Table of direction between any two chessboard squares, initialised
// in scid_Init():
directionT sqDir[66][66];


//////////////////////////////////////////////////////////////////////
//   Scid Initialisation Routine

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// scid_Init():
//      Must be called before move generation etc is done. Sets up
//      any uninitialised piece/square/move tables.
//
void
scid_Init()
{
    // Check that we only call this once:
    static bool init = false;
    if(init)
    {
        return;
    }
    init = true;

    // Initialise the sqDir[][] array of directions between every pair
    // of squares.
    squareT i, j;
    directionT dirArray[] = { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT,
                              DOWN_LEFT, DOWN_RIGHT, NULL_DIR
                            };
    // First, set everything to NULL_DIR:
    for(i = A1; i <= NULL_SQUARE; i++)
    {
        for(j = A1; j <= NULL_SQUARE; j++)
        {
            sqDir[i][j] = NULL_DIR;
        }
    }
    // Now fill in the valid directions:
    for(i = A1; i <= H8; i++)
    {
        directionT * dirptr = dirArray;
        while(*dirptr != NULL_DIR)
        {
            j = square_Move(i, *dirptr);
            while(j != NULL_SQUARE)
            {
                sqDir[i][j] = *dirptr;
                j = square_Move(j, *dirptr);
            }
            dirptr++;
        }
    }
}


} // End namespace Guess

//////////////////////////////////////////////////////////////////////
//  EOF: misc.cpp
//////////////////////////////////////////////////////////////////////

