///////////////////////////////////////////////////////////////////////////
//
//  FILE:       textbuf.h
//              TextBuffer class
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    2.7
//
//  Notice:     Copyright (c) 1999-2001 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
///////////////////////////////////////////////////////////////////////////


#ifndef SCID_TEXTBUF_H
#define SCID_TEXTBUF_H

#include "common.h"
#include <stdio.h>

class TextBuffer
{
private:
    //----------------------------------
    //  TextBuffer:  Data Structures
    
    uint   Column;
    uint   IndentColumn;
    uint   WrapColumn;
    uint   LineIsEmpty;  // true if current line is empty.
    uint   LineCount;
    uint   ByteCount;
    uint   BufferSize;
    bool   ConvertNewlines;  // If true, convert newlines to spaces.
    char * Buffer;
    char * Current;

    bool   PausedTranslations;
    bool   HasTranslations;
    const char * Translation [256];

    inline void   AddChar (char ch);
    TextBuffer(const TextBuffer&);
    TextBuffer& operator=(const TextBuffer&);

    //----------------------------------
    //  TextBuffer:  Public Functions
public:
    TextBuffer() {
        Init();
        SetBufferSize(1280000);
    }
    ~TextBuffer()   { Free(); }
    
    void     Init ();
    void     Free ();
    void     Empty ();
    
    void     SetBufferSize (uint length);
    uint     GetBufferSize()     { return BufferSize; }
    uint     GetByteCount()      { return ByteCount; }
    uint     GetLineCount()      { return LineCount; }
    uint     GetColumn()         { return Column; }
    uint     GetWrapColumn ()    { return WrapColumn; }
    void     SetWrapColumn (uint column) { WrapColumn = column; }
    uint     GetIndent ()        { return IndentColumn; }
    void     SetIndent (uint column) { IndentColumn = column; }
    char *   GetBuffer ()        { return Buffer; }
    void     NewlinesToSpaces (bool b) { ConvertNewlines = b; }

    void     AddTranslation (char ch, const char * str);
    // void     ClearTranslation (char ch) { Translation[ch] = NULL; }
    // Changed ch to int, to avoid compiler warnings. 
    void     ClearTranslation (int ch) { Translation[ch] = NULL; }
    void     ClearTranslations () { HasTranslations = false; }
    void     PauseTranslations () { PausedTranslations = true; }
    void     ResumeTranslations () { PausedTranslations = false; }

    errorT   NewLine();
    errorT   Indent();
    errorT   PrintLine (const char * str);
    errorT   PrintWord (const char * str);
    errorT   PrintString (const char * str);
    errorT   PrintSpace ();
    errorT   PrintChar (char b);

    errorT   PrintInt (uint i, const char * str);
    inline errorT PrintInt (uint i) { return PrintInt (i, ""); }

};

inline void
TextBuffer::AddChar (char ch)
{
    if (HasTranslations  &&  !PausedTranslations) {
        byte b = (byte) ch;
        const char * str = Translation[b];
        if (str != NULL) {
            const char * s = str;
            while (*s) {
                *Current++ = *s++;
                ByteCount++;
            }
            return;
        }
    }
    *Current = ch;
    Current++;
    ByteCount++;
}

#endif  // SCID_TEXTBUF_H

///////////////////////////////////////////////////////////////////////////
//  EOF: textbuf.h
///////////////////////////////////////////////////////////////////////////

