//////////////////////////////////////////////////////////////////////
//
//  FILE:       dstring.cpp
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


#include "dstring.h"
#include "misc.h"

namespace Guess
{

void
DString::Init (uint capacity)
{
    if (capacity < DSTRING_MinCapacity) { capacity = DSTRING_MinCapacity; }
    Start = new char [capacity];
    Start[0] = 0;
    Len = 0;
    Capacity = capacity;
}


void
DString::Extend (uint neededLength)
{
    ASSERT (Len < Capacity  &&  Start[Len] == 0);

    if (Capacity > neededLength) { return; }

    // Double the capacity of the string until it is long enough:
    uint newCapacity = Capacity;
    while (newCapacity <= neededLength) { newCapacity += newCapacity; }

    char * newStart = new char [newCapacity];
    for (uint i=0; i <= Len; i++) { newStart[i] = Start[i]; }
    delete[] Start;
    Start = newStart;
    Capacity = newCapacity;
    return;
}

void
DString::AddChar (char ch)
{
    Extend (Len+1);
    Start[Len] = ch;
    Len++;
    Start[Len] = 0;
}

void
DString::AppendUint (uint i)
{
    char s[16];
    sprintf (s, "%u", i);
    Append (s);
}

void
DString::AppendInt (int i)
{
    char s[16];
    sprintf (s, "%d", i);
    Append (s);
}

void
DString::Append (const char * str)
{
    uint addLength = strLength (str);
    uint newLength = Len + addLength;

    Extend (newLength);
    char * endPtr = &(Start[Len]);
    while (addLength-- > 0) { *endPtr++ = *str++; }
    ASSERT (*str == 0);
    *endPtr = 0;
    Len = newLength;
}

// To allow convenient appending of multiple strings without resorting
// to messy variable-length argument lists, we define DString::Append()
// for up to five string arguments, and for up to four arguments where
// one is an unsigned integer and the rest are strings.

void DString::Append (uint i)
{ AppendUint (i); }

void DString::Append (const char * s1, const char * s2)
{ Append (s1);  Append (s2); }
void DString::Append (const char * s1, uint i2)
{ Append (s1);  Append (i2); }
void DString::Append (uint i1, const char * s2)
{ Append (i1);  Append (s2); }

void DString::Append (const char * s1, const char * s2, const char * s3)
{ Append (s1);  Append (s2);  Append (s3); }
void DString::Append (const char * s1, const char * s2, uint i3)
{ Append (s1);  Append (s2);  Append (i3); }
void DString::Append (const char * s1, uint i2, const char * s3)
{ Append (s1);  Append (i2);  Append (s3); }
void DString::Append (uint i1, const char * s2, const char * s3)
{ Append (i1);  Append (s2);  Append (s3); }

void DString::Append (const char * s1, const char * s2, const char * s3,
                      const char * s4)
{ Append (s1);  Append (s2);  Append (s3);  Append (s4); }
void DString::Append (const char *s1, const char *s2, const char *s3, uint i4)
{ Append (s1);  Append (s2);  Append (s3);  Append (i4); }
void DString::Append (const char *s1, const char *s2, uint i3, const char *s4)
{ Append (s1);  Append (s2);  Append (i3);  Append (s4); }
void DString::Append (const char *s1, uint i2, const char *s3, const char *s4)
{ Append (s1);  Append (i2);  Append (s3);  Append (s4); }
void DString::Append (uint i1, const char *s2, const char *s3, const char *s4)
{ Append (i1);  Append (s2);  Append (s3);  Append (s4); }

void DString::Append (const char * s1, const char * s2, const char * s3,
                      const char * s4, const char * s5)
{ Append (s1);  Append (s2);  Append (s3);  Append (s4);  Append (s5); }


#if 0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// DString::ReadLine():
//    Reads a line from an open file into the DString.
//    Does not copy the end-of-line character into the string,
//    and filters out ASCII-13 characters that precede a newline
//    in DOS files.
void
DString::ReadLine (MFile * fp)
{
    ASSERT (fp != NULL);
    int ch = fp->ReadOneByte();
    while (ch != '\n'  &&  ch != EOF) {
        if (ch != '\r') {
            AddChar (ch);
        }
        ch = fp->ReadOneByte();
    }
}
#endif


} // End namespace Guess

///////////////////////////////////////////////////////////////////////////
//  EOF: dstring.cpp
///////////////////////////////////////////////////////////////////////////
