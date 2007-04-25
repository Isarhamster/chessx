//////////////////////////////////////////////////////////////////////
//
//  FILE:       dstring.h
//              Dynamic String class
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    2.2
//
//  Notice:     Copyright (c) 2000  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


// A DString is a string that can extend its allocated space when necessary
// and is efficient for append operations since the end of the string is
// remembered.
// The allocated space for the string is doubled whenever necessary, so it
// grows efficiently (at the cost of some wasted space) even for very large
// strings.

#ifndef SCID_DSTRING_H
#define SCID_DSTRING_H

#include "common.h"

namespace Guess
{

const uint DSTRING_MinCapacity = 64;

class DString
{
  private:
    char * Start;
    uint   Len;
    uint   Capacity;

    void   Init (uint capacity);
    void   Extend (uint neededLength);

public:

    DString() { Init (DSTRING_MinCapacity); }
    DString (uint size) { Init (size); }
    ~DString() { delete[] Start; }

    void Clear (void)        { Start[0] = 0; Len = 0; }
    const char * Data (void) { return (const char *) Start; }
    uint Length (void)       { return Len; }

    void AddChar (char ch);
    void AppendUint (uint i);
    void AppendInt (int i);
    void Append (const char * str);
    void Append (uint i);
    void Append (const char * s1, const char * s2);
    void Append (const char * s1, uint i2);
    void Append (uint i1, const char * s2);
    void Append (uint i1, uint i2);
    void Append (const char * s1, const char * s2, const char * s3);
    void Append (const char * s1, const char * s2, uint i3);
    void Append (const char * s1, uint i2, const char * s3);
    void Append (uint i1, const char * s2, const char * s3);

    void Append (const char * s1, const char * s2, const char * s3,
                 const char * s4);
    void Append (const char * s1, const char * s2, const char * s3, uint i4);
    void Append (const char * s1, const char * s2, uint i3, const char * s4);
    void Append (const char * s1, uint i2, const char * s3, const char * s4);
    void Append (uint i1, const char * s2, const char * s3, const char * s4);
    void Append (const char * s1, const char * s2, const char * s3,
                 const char * s4, const char * s5);
};

} // End namespace Guess

#endif  // SCID_DSTRING_H

