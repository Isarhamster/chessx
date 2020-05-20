///////////////////////////////////////////////////////////////////////////
//
//  FILE:       textbuf.cpp
//              TextBuffer class methods
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    2.7
//
//  Notice:     Copyright (c) 1999-2001 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
///////////////////////////////////////////////////////////////////////////

#include "textbuf.h"
#include "misc.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::Init(): Initialise the textbuffer.
void
TextBuffer::Init (void)
{
    BufferSize = Column = IndentColumn = LineCount = ByteCount = 0;
    LineIsEmpty = 1;
    Buffer = Current = NULL;
    WrapColumn = 80;
    ConvertNewlines = true;
    HasTranslations = false;
    PausedTranslations = false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::Free(): Free the TextBuffer.
void
TextBuffer::Free (void)
{
    if (Buffer != NULL) {
#ifdef WINCE
        my_Tcl_Free( Buffer);
#else
        delete[] Buffer;
#endif
        Buffer = NULL;
        BufferSize = 0;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::Empty(): Empty the TextBuffer.
void
TextBuffer::Empty (void)
{
    ASSERT(Buffer != NULL);
    ByteCount = Column = LineCount = 0; LineIsEmpty = 1;
    Current = Buffer;
    *Current = 0;
    ConvertNewlines = true;
    HasTranslations = false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TextBuffer::AddTranslation():
//   Adds a translation for a character.
//   The translation string will be printed in place of that character.
void
TextBuffer::AddTranslation (char ch, const char * str)
{
    if (! HasTranslations) {
        HasTranslations = true;
        for (uint i=0; i < 256; i++) {
            Translation [i] = NULL;
        }
    }
    Translation [(byte) ch] = str;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::SetBufferSize(): Set the buffer size.
void
TextBuffer::SetBufferSize (uint length)
{
#ifdef WINCE
    if (Buffer != NULL) { my_Tcl_Free( Buffer); }
    Buffer = my_Tcl_Alloc(sizeof(char[length]));
#else
    if (Buffer != NULL) { delete[] Buffer; }
    Buffer = new char[length];
#endif
    BufferSize = length;
    ByteCount = Column = LineCount = 0; LineIsEmpty = 1;
    Current = Buffer;
    *Current = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::NewLine(): Add a newline.
errorT
TextBuffer::NewLine ()
{
    ASSERT (Current != NULL);
    if (ByteCount >= BufferSize) { return ERROR_BufferFull; }
    *Current++ = '\n'; 
    LineCount++; ByteCount++; LineIsEmpty = 1;
    Column = 0; 
    while (Column < IndentColumn) {
        if (ByteCount >= BufferSize) { return ERROR_BufferFull; }
        *Current++ = ' '; Column++; ByteCount++;
    }
    *Current = 0;
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::Indent(): Indent to the current Indentation level..
errorT
TextBuffer::Indent ()
{
    ASSERT (Current != NULL);
    if (!LineIsEmpty) {
        return NewLine();
    } else {
        while (Column < IndentColumn) {
            if (ByteCount >= BufferSize) { return ERROR_BufferFull; }
            *Current++ = ' '; Column++; ByteCount++;
        }
        *Current = 0;
    }
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::PrintLine(): Print a string then newline. Does not
//          check for the line going past WrapColumn.
errorT
TextBuffer::PrintLine (const char * str)
{
    ASSERT(Current != NULL);
    while (*str != 0) {
        if (ByteCount > BufferSize) { return ERROR_BufferFull; }
        AddChar (*str);
        str++;
    }
    return NewLine();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::PrintWord(): Prints a word, wrapping if necessary.
//     It does NOT add a space, since that is left to the caller to
//     provide in the string.
errorT
TextBuffer::PrintWord (const char * str)
{
    ASSERT(Current != NULL);
    uint length = strLength (str);
    if (Column + length >= WrapColumn)    { NewLine(); }
    if (ByteCount + length >= BufferSize) { return ERROR_BufferFull; }
    while (*str != 0) {
        char ch = *str;
        AddChar (ch);
        str++;
        Column++;
    }
    *Current = 0;  // add trailing end-of-string to buffer
    LineIsEmpty = 0;
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::PrintSpace(): Prints a space OR a newline character,
//     but not both.
errorT
TextBuffer::PrintSpace (void)
{
    if (ByteCount + 1 >= BufferSize)  { return ERROR_BufferFull; }
    if (Column + 1 >= WrapColumn) {
        NewLine();
    } else {
        *Current = ' '; Current++; ByteCount++; Column++; LineIsEmpty = 0;
    }
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::PrintChar(): prints a single char, adding a newline
//      first if necessary.
errorT
TextBuffer::PrintChar (char b)
{
    if (Column + 1 >= WrapColumn)  { NewLine(); }
    if (ByteCount + 1 >= BufferSize)  { return ERROR_BufferFull; }
    AddChar (b);
    Column++; LineIsEmpty = 0;
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::PrintString(): Print a string, wrapping at spaces.
//      Also converts newlines in the string into spaces.
errorT
TextBuffer::PrintString (const char * str)
{
    errorT err;
    char currentWord[1024];  // should be long enough for a word
    while (*str != 0) {
        char * b = currentWord;
        *b = 0;
        // get next word and print it:
        while (*str != ' '  && *str != '\n'  &&  *str != '\0') {
            *b = *str; b++; str++;
        }
        // end of word/line/text reached
        *b = 0;
        err = PrintWord (currentWord);
        if (err != OK) { return err; }
        if (*str == 0) { return OK; }
        if (*str == '\n'  &&  !ConvertNewlines) {
            err = NewLine();
        } else {
            err = PrintSpace();
        }
        if (err != OK) { return err; }
        str++;
    }
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### TextBuffer::PrintInt(): Print a decimal number followed by string
//      as a word (so it appends a space at the end and wraps if
//      necessary).
errorT
TextBuffer::PrintInt (uint i, const char * str)
{
    char temp[255];
    sprintf(temp, "%d%s", i, str);
    return PrintWord(temp);
}

///////////////////////////////////////////////////////////////////////////
//  EOF: textbuf.cpp
///////////////////////////////////////////////////////////////////////////
