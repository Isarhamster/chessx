//////////////////////////////////////////////////////////////////////
//
//  FILE:       myassert.cpp
//              Function to handle failed ASSERTs
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    0.3
//
//  Notice:     Copyright (c) 1999  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <stdio.h>
#include "myassert.h"

int numAsserts = 0;

void _MyAssert(char *filename, unsigned linenum)
{
    fflush (NULL);  // Flush all buffers first.
    fprintf (stderr, "\nASSERT FAILED!!: %s, line %u\n", filename, linenum);
    fflush (stderr);
    abort ();
}

//////////////////////////////////////////////////////////////////////
//  EOF: myassert.cpp
//////////////////////////////////////////////////////////////////////

