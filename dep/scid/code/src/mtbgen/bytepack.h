//////////////////////////////////////////////////////////////////////
//
//  FILE:       bytepack.h
//              Simple fast sliding window (LZ77) compressor
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2003 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#ifndef SCID_BYTEPACK_H
#define SCID_BYTEPACK_H

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BytePacker
//   This class implements a simple fast byte-oriented LZ77 (sliding
//   window) compression algorithm. The compressed format is made up
//   of control bytes and literal/match data bytes. For each symbol
//   in the compressed representation, there is one control bit to
//   indicate whether it is a literal or match. Literals are encoded
//   in one byte (a variable-bit code would give better compression
//   but be slower), while matches are encoded in 1, 2 or 3 bytes,
//   depending on the length of the match and whether the offset is
//   one that has been seen recently.
//
//   To speed up compression, two structures are used. FirstBackMatch
//   stores the location of the most recent previous occurrence of
//   each byte value. NextBackMatch is then used to find earlier
//   occurrences within the sliding window.

class BytePacker
{
  public:
    static const uint MaxBufferSize = 1 << 24;
    static const uint OverflowBytes = 8;

  private:

    static const byte FLAG_Packed = 1;     // Buffer is stored packed.
    static const byte FLAG_Copied = 0;     // Buffer is stored uncompressed.

    static const uint LengthBits = 4;
    static const uint LengthMask = ((1 << LengthBits) - 1);
    static const uint ShortOffsetBits = 7 - LengthBits;
    static const uint ExtraOffsetBits = 8;
    static const uint OffsetBits = ShortOffsetBits + ExtraOffsetBits;
    static const uint ShortOffsetThreshold = 1 << ShortOffsetBits;
    static const uint ShortOffsetMask = ShortOffsetThreshold - 1;

    static const uint WindowSize = 1 << OffsetBits;
    static const uint MaxOffset = (WindowSize - 1);

    static const uint ShortestMatchLength = 3;
    static const uint ShortMatchThreshold = (LengthMask + ShortestMatchLength);
    static const uint MaxMatchLength = (ShortMatchThreshold + 255);
    static const uint AcceptableMatchLength = 256;
    static const uint MaxMatchAttempts = WindowSize;

    ushort NextBackMatch [WindowSize];
    ushort FirstBackMatch [256];
    byte FullUpdates;

    inline void
    UpdateMatchTables (const byte * currentPtr, uint currentBytes)
    {
        byte b = *currentPtr;
        NextBackMatch[currentBytes & MaxOffset] = FirstBackMatch[b];
        FirstBackMatch[b] = currentBytes;
    }

    inline uint
    BestMatch (const byte * buffer, uint current, uint bufLength, uint * matchPos)
    {
        ASSERT (buffer != NULL  &&  matchPos != NULL);
        ASSERT (current < bufLength);

        const byte * currentPtr = buffer + current;
        uint bestLength = 0;
        int bestLocation = 0;
        int minLocation = (int)current - MaxOffset;
        if (minLocation < 0) { minLocation = 0; }

        int oldLocation = current;
        byte b = *currentPtr;
        int location = FirstBackMatch[b];
        int attempts = MaxMatchAttempts;

        // Determine the maximum match length:
        uint maxMatchLength = bufLength - current;
        if (maxMatchLength > MaxMatchLength) { maxMatchLength = MaxMatchLength; }

        while (location < oldLocation  &&  location >= minLocation  &&  attempts)
        {
            attempts--;
            uint mlen = 0;
            const byte * cptr = currentPtr;
            const byte * backptr = buffer + location;
            while ((mlen < maxMatchLength)  &&  (*cptr++ == *backptr++)) {
                mlen++;
            }
            if (mlen > bestLength) {
                bestLength = mlen;
                bestLocation = location;
                // Stop now if this match is long enough:
                if (bestLength >= AcceptableMatchLength) { break; }
            }
            // Update the search location:
            oldLocation = location;
            location = NextBackMatch [location & MaxOffset];
        }

        // Update the match tables:
        NextBackMatch [current & MaxOffset] = FirstBackMatch [b];
        FirstBackMatch [b] = current;

        // Return the best match size and location:
        *matchPos = bestLocation;
        return bestLength;
    }

  public:
    BytePacker()
    {
        FullUpdates = true;
    }
    ~BytePacker() {}

    uint Pack (const byte * inBuffer, byte * outBuffer, uint inLength);
    errorT Unpack (const byte * inBuffer, byte * outBuffer,
                   uint inLength, uint outLength);
};

uint
BytePacker::Pack (const byte * inBuffer, byte * outBuffer, uint inLength)
{
    ASSERT (inBuffer != NULL  &&  outBuffer != NULL);
    ASSERT (inLength <= MaxBufferSize);

#define ENCODE_CONTROL_BIT(bit) \
    ASSERT (bit == 0  ||  bit == 1);  \
    control = (control << 1) | (bit); \
    controlBits--;                    \
    if (controlBits == 0) {           \
        *controlPtr = control;        \
        controlPtr = outPtr++;        \
        outBytes++;                   \
        control = 0;                  \
        controlBits = 8;              \
    }

    const byte * inPtr = inBuffer;
    byte * outPtr = outBuffer + 2;
    byte * controlPtr = outBuffer + 1;
    const byte * endPtr = inBuffer + inLength;

    uint inBytes = 0;
    uint outBytes = 2;
    byte control = 0;
    uint controlBits = 8;
    outBuffer[0] = FLAG_Packed;

    uint prevOffsets[ShortOffsetThreshold] = {0};
    uint numMatchesSeen = 0;
    uint numLiterals = 0;

    while (inPtr < endPtr  &&  outBytes <= inLength) {

        // Find the best match and its length:
        uint matchPos = 0;
        uint matchLength = BestMatch (inBuffer, inBytes, inLength, &matchPos);

        if (matchLength >= ShortestMatchLength) {
            // Encode a sliding window match:
            ENCODE_CONTROL_BIT (1);

            uint offset = inBytes - matchPos;
            ASSERT (offset > 0 &&  offset < WindowSize);

            // Look for an offset amount that has occurred recently,
            // it will save one byte over the regular offset encoding:
            bool isCommonOffset = false;
            uint commonOffsetIndex = 0;
            for (uint i=0; i < ShortOffsetThreshold; i++) {
                if (offset == prevOffsets[i]) {
                    commonOffsetIndex = i;
                    isCommonOffset = true;
                    break;
                }
            }
            // Update the array of recent offsets. Note that we allow
            // duplicate values, just for speed and simplicity.
            prevOffsets[numMatchesSeen & ShortOffsetMask] = offset;
            numMatchesSeen++;
            if (isCommonOffset) {
               offset = commonOffsetIndex;
            }

            // Encode the data bytes for this match. There will be a code
            // byte, then possibly an extra offset byte, then finally a
            // possible extra match byte, for a total of one to three bytes.

            byte code = (offset & ShortOffsetMask) << LengthBits;
            if (! isCommonOffset) {
                // Set the top bit of the code byte, showing a long offset:
                code |= 128;
            }
            if (matchLength >= ShortMatchThreshold) {
                code |= (ShortMatchThreshold - ShortestMatchLength);
            } else {
                code |= (matchLength - ShortestMatchLength);
            }

            *outPtr++ = code;
            outBytes++;

            // Encode the extra offset byte if necessary:
            if (! isCommonOffset) {
                *outPtr++ = (byte) (offset >> ShortOffsetBits);
                outBytes++;
            }

            // Encode the extra match byte if necessary:
            if (matchLength >= ShortMatchThreshold) {
                *outPtr++ = (byte) (matchLength - ShortMatchThreshold);
                outBytes++;
            }

            // If FullUpdates is set, update the match tables
            // for each character within the matching phrase (otherwise,
            // the tables are only updated at the start of each matching
            // phrase and at each literal). With FullUpdates, compression
            // is better but slower.

            if (FullUpdates) {
                for (uint i=1; i < matchLength; i++) {
                    UpdateMatchTables (inPtr + i, inBytes + i);
                }
            }

            inBytes += matchLength;
            inPtr += matchLength;

        } else {
            // There was no match, so output a literal byte:
            ENCODE_CONTROL_BIT (0);
            *outPtr++ = *inPtr++;
            outBytes++;
            inBytes++;
            numLiterals++;
        }
    } // End of main loop

    // Flush the final control bits:
    control <<= controlBits;
    *controlPtr = control;

    // Check for expansion, and do a regular copy if necessary:
    if (outBytes > inLength) {
        outBuffer[0] = FLAG_Copied;

        // Copy the inBuffer to outBuffer, after the initial flag byte:
        const byte * in = inBuffer;
        byte * out = outBuffer + 1;
        uint len = inLength;
        while (len--) { *out++ = *in++; }
        return inLength + 1;
    }

    return outBytes;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BytePacker::Unpack
//   Unpacks the compressed data in inBuffer to outBuffer.
errorT
BytePacker::Unpack (const byte * inBuffer, byte * outBuffer,
                    uint inLength, uint outLength)
{
    ASSERT (inBuffer != NULL  &&  outBuffer != NULL);

    if (inLength == 0  ||  inLength > MaxBufferSize+1) {
        return ERROR_Corrupt;
    }
    if (outLength > MaxBufferSize) { return ERROR_Corrupt; }

    uint prevOffsets[ShortOffsetThreshold] = {0};
    uint numMatchesSeen = 0;

    const byte * inPtr = inBuffer + 1;
    byte * outPtr = outBuffer;
    int inBytesLeft = inLength - 1;
    int outBytesLeft = outLength;

    // Check if the buffer was copied without compression:
    if (inBuffer[0] == FLAG_Copied) {
        // outLength *must* be one byte shorter than inLength:
        if (outLength + 1 != inLength) { return ERROR_Corrupt; }

        while (inBytesLeft--) { *outPtr++ = *inPtr++; }
        return OK;
    }

    if (*inBuffer != FLAG_Packed) { return ERROR_Corrupt; }

    uint control = *inPtr++;
    uint controlBits = 8;
    inBytesLeft--;

    while (outBytesLeft > 0) {
        // Get the next control bit:
        uint bitFlag = control & 0x80;
        control <<= 1;
        controlBits--;
        if (controlBits == 0) {
            control = *inPtr++;
            inBytesLeft--;
            controlBits = 8;
        }

        if (bitFlag) {
            // Expand the match:
            byte code = *inPtr++;
            inBytesLeft--;
            uint len = (code & LengthMask) + ShortestMatchLength;
            uint offset = (code >> LengthBits) & ShortOffsetMask;
            if (code & 128) {
                // Read extra offset bits:
                uint extra = *inPtr++;
                inBytesLeft--;
                offset |= extra << ShortOffsetBits;
            } else {
                offset = prevOffsets[offset];
            }
            prevOffsets[numMatchesSeen & ShortOffsetMask] = offset;
            numMatchesSeen++;

            // Offsets of value 0 are possible in the current coding
            // method, but not used to denote anything yet, so they
            // are not accepted as clean data:
            if (offset == 0) { return ERROR_Corrupt; }

            if (len == ShortMatchThreshold) {
                // It is a long length; read the extra match bits:
                len = (*inPtr++) + ShortMatchThreshold;
                inBytesLeft--;
            }
            outBytesLeft -= len;
            if (outBytesLeft < 0) { return ERROR_Corrupt; }

            // Copy the match to the end of the out buffer:
            byte * backPtr = outPtr - offset;
            // Unroll the loop for long matches:
            while (len >= 8) {
                *outPtr++ = *backPtr++;  *outPtr++ = *backPtr++;
                *outPtr++ = *backPtr++;  *outPtr++ = *backPtr++;
                *outPtr++ = *backPtr++;  *outPtr++ = *backPtr++;
                *outPtr++ = *backPtr++;  *outPtr++ = *backPtr++;
                len -= 8;
            }
            // Copy a short match or the last bytes of a long match:
            while (len--) { *outPtr++ = *backPtr++; }

        } else {
            // Copy the literal character:
            *outPtr++ = *inPtr++;;
            inBytesLeft--;
            outBytesLeft--;
        }
    }

    // Check the buffers are both fully used:
    if (outBytesLeft != 0  ||  inBytesLeft != 0) {
        return ERROR_Corrupt;
    }
    return OK;
}

#endif // SCID_BYTEPACK_H

//////////////////////////////////////////////////////////////////////
//  EOF: bytepack.h
//////////////////////////////////////////////////////////////////////
