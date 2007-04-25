//////////////////////////////////////////////////////////////////////
//
//  FILE:       myassert.h
//              ASSERT statement macro
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    2.3
//
//  Notice:     Copyright (c) 2001  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//  ASSERT MACRO

// ASSERT macro: asserts an expression. Differs from the standard
//    assert in that it does NOT print the expression (this is a waste,
//    if an assert fails you can go to the code to see why) and that
//    it MUST be a statement, not part of a larger expression.
//    Adapted from the book "Writing Solid Code".

#ifndef MYASSERT_H

namespace Guess
{

extern int numAsserts;

#ifdef ASSERTIONS

    void _MyAssert(char *, unsigned);

    #define ASSERT(f)               \
        numAsserts++;               \
        if (f)                      \
            {}                      \
        else                        \
            _MyAssert(__FILE__, __LINE__)

#else   // #ifndef ASSERTIONS

    #define ASSERT(f) 

#endif  // #ifdef ASSERTIONS

} // End namespace Guess

#endif  // #ifndef MYASSERT_H


//////////////////////////////////////////////////////////////////////
//  EOF:    myassert.h
//////////////////////////////////////////////////////////////////////

