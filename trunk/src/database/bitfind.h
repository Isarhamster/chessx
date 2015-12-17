/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <qglobal.h>

#ifndef BITFIND_H
#define BITFIND_H

template <typename T>
T getFirstBitAndClear64(quint64& bb)
{
    quint64 x = bb & -(qint64)bb;
    bb ^= x;
#ifdef __GNUG__
    return T(x ? (63 - __builtin_clzll(x)) : 0);
#elif _MSC_VER
#ifdef __x86_64__
    if(x)
    {
        unsigned long r;
        _BitScanReverse64(&r, x);
        return T(r);
    }
    return a1;
#else
    if(x)
    {
        unsigned long r;
        unsigned long y = (x >> 32);
        if(y)
        {
            _BitScanReverse(&r, y);
            return T(32 + r);
        }
        _BitScanReverse(&r, x);
        return T(r);
    }
    return a1;
#endif
#else
    // SBE - After a fair bit of testing, this is the fastest portable version
    // i could come up with, it's about twice as fast as shift-testing 64 times.
    unsigned int r =  0;
    if(!(x & 0xffffffff))
    {
        x >>= 32;
        r |= 32;
    }
    if(!(x & 0xffff))
    {
        x >>= 16;
        r |= 16;
    }
    if(!(x & 0xff))
    {
        x >>= 8;
        r |= 8;
    }
    if(!(x & 0xf))
    {
        x >>= 4;
        r |= 4;
    }
    if(!(x & 0x3))
    {
        x >>= 2;
        r |= 2;
    }
    if(!(x & 0x1))
    {
        r |= 1;
    }
    return T(r);
#endif
}

#endif // BITFIND_H
