//////////////////////////////////////////////////////////////////////
//
//  FILE:       misc.h
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


#ifndef SCID_MISC_H
#define SCID_MISC_H

#include "common.h"
#include "error.h"
#include <ctype.h>   // For isspace(), etc

namespace Guess
{

// Scid initialisation routine: this MUST be called before such things as
// computing chess moves are done, since it sets up piece movement tables.
//
    void   scid_Init();
} // End namespace Guess

#endif  // #ifdef SCID_MISC_H

//////////////////////////////////////////////////////////////////////
//  EOF: misc.h
//////////////////////////////////////////////////////////////////////

