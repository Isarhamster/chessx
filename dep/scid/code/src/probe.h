//////////////////////////////////////////////////////////////////////
//
//  FILE:       probe.h
//              Header file for Scid interface to Tablebase decoder
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.4
//
//  Notice:     Copyright (c) 2000-2002 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#ifndef SCID_PROBE_H
#define SCID_PROBE_H

#include "position.h"
#include "matsig.h"

bool scid_TB_compiled (void);

uint scid_TB_MaxPieces (void);

uint scid_TB_CacheSize (void);

void scid_TB_SetCacheSize (uint cachesize);

uint scid_TB_Init (const char * dir);

bool scid_TB_Available (matSigT matsig);

errorT scid_TB_Probe (Position * pos, int * score);

#endif  // #ifdef SCID_PROBE_H

//////////////////////////////////////////////////////////////////////
/// END of probe.h
//////////////////////////////////////////////////////////////////////
