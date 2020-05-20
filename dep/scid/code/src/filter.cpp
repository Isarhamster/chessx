//////////////////////////////////////////////////////////////////////
//
//  FILE:       filter.cpp
//              Filter and CompressedFilter Classes
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    1.9
//
//  Notice:     Copyright (c) 2000  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#include "tree.h"
#include <cstring>


//////////////////////////////////////////////////////////////////////
//
// CompressedFilter methods


static uint
packBytemap (const byte * inBuffer, byte * outBuffer, uint inLength);

static errorT
unpackBytemap (const byte * inBuffer, byte * outBuffer,
               uint inLength, uint outLength);

// OVERFLOW_BYTES:
//      The maximum length that the output buffer could exceed the input
//      buffer by when compressing. Since a long run length can take six
//      bytes and the control byte could be encoded in the same step,
//      seven bytes is sufficient -- so use 8 for nice alignment.
//
const uint OVERFLOW_BYTES = 8;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CompressedFilter::Verify():
//      Return OK only if the compressed filter is identical to
//      the regular filter passed as the parameter.
//
errorT
CompressedFilter::Verify (Filter * filter)
{
    if (CFilterSize != filter->Size()) { return ERROR_Corrupt; }

    // Decompress the compressed block and compare with the original:
    byte * tempBuffer = new byte [CFilterSize];
    const byte * filterData = filter->data();

    if (unpackBytemap (CompressedData, tempBuffer,
                       CompressedLength, CFilterSize) != OK) {
        delete[] tempBuffer;
        return ERROR_Corrupt;
    }
    for (uint i=0; i < CFilterSize; i++) {
        if (tempBuffer[i] != filterData[i]) {
            delete[] tempBuffer;
            return ERROR_Corrupt;
        }
    }
    delete[] tempBuffer;

    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CompressedFilter::CompressFrom():
//      Sets the compressed filter to be the compressed representation
//      of the supplied filter.
//
void
CompressedFilter::CompressFrom (Filter * filter)
{
    delete[] CompressedData;

    CFilterSize = filter->Size();
    if(filter->data() == NULL) {
        CompressedLength = 0;
        CompressedData = NULL;
        return;
    }
    byte * tempBuf = new byte [CFilterSize + OVERFLOW_BYTES];
    CompressedLength = packBytemap (filter->data(), tempBuf, CFilterSize);
    CompressedData = new byte [CompressedLength];
    std::memcpy (CompressedData, tempBuf, CompressedLength);
    delete[] tempBuf;

    // Assert that the compressed filter decompresses identical to the
    // original, is assertions are being tested:
    ASSERT (Verify (filter) == OK);

    return;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CompressedFilter::UncompressTo():
//      Sets the supplied filter to contain the uncompressed data
//      stored in this compressed filter.
//
errorT
CompressedFilter::UncompressTo (Filter * filter) const
{
    // The filter and compressed filter MUST be of the same size:
    if (CFilterSize != filter->Size()) { return ERROR_Corrupt; }
    if (CompressedLength == 0) {
        filter->Init(CFilterSize);
        return OK;
    }

    byte * tempBuffer = new byte [CFilterSize];
    if (unpackBytemap (CompressedData, tempBuffer,
                       CompressedLength, CFilterSize) != OK) {
        delete[] tempBuffer;
        return ERROR_Corrupt;
    }
    for (uint index=0; index < CFilterSize; index++) {
        filter->Set (index, tempBuffer[index]);
    }
    delete[] tempBuffer;
    return OK;
}


const byte FLAG_Packed = 1;     // Indicates buffer is stored packed.
const byte FLAG_Copied = 0;     // Indicates buffer is stored uncompressed.

const uint CODE_ZeroLiteral = 0;
const uint CODE_PrevLiteral = 1;
const uint CODE_RunLength = 2;
const uint CODE_NewLiteral = 3;

const uint MIN_RLE_LENGTH = 9;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// packBytemap():
//      Compresses the contents of inBuffer to outBuffer using a tailored
//      run-length encoding and byte packing algorithm.
//
//      The compression algorithm assumes:
//        -  that the byte value 0 is very common;
//        -  that the input buffer usually contains just two values,
//      which is typically the case for tree filters.
//
//      At each step through the algorithm, one of the following is coded:
//        - a run length of 9 or more of the same value is coded in 18
//            bits (or 50 bits if the length is >= 255); or
//        - a byte with value zero is coded in two bits; or
//        - a byte with nonzero value the same as the last nonzero byte
//            (excluding run lengths) is coded in two bits; or
//        - a byte with nonzero value different to the last nonzero byte
//            is coded in 10 bits.
//
//      The length of the output buffer is returned. It will never be
//      larger than (inLength + 1), but up to (inLength + OVERFLOW_BYTES)
//      bytes of outBuffer could be used temporarily before the length
//      is checked, so outBuffer must be at least (inLength + OVERFLOW_BYTES)
//      bytes long for safety.
//
static uint
packBytemap (const byte * inBuffer, byte * outBuffer, uint inLength)
{
    ASSERT (inBuffer != NULL  &&  outBuffer != NULL);

    byte prevLiteral = 0;
    const byte * inPtr = inBuffer;
    byte * outPtr = outBuffer + 2;
    byte * controlPtr = outBuffer + 1;
    const byte * endPtr = inBuffer + inLength;

    uint outBytes = 2;
    uint controlData = 0;
    uint controlBits = 8;

    uint stats[4] = {0, 0, 0, 0};

#define ENCODE_CONTROL_BITS(bits)       \
    ASSERT (bits >= 0  &&  bits <= 3);  \
    controlData >>= 2;                  \
    controlData |= (bits << 6);         \
    stats[bits]++;                      \
    ASSERT (controlBits >= 2);          \
    controlBits -= 2;                   \
    if (controlBits == 0) {             \
        *controlPtr = controlData;      \
        controlPtr = outPtr++;          \
        outBytes++;                     \
        controlData = 0;                \
        controlBits = 8;                \
    }

    outBuffer[0] = FLAG_Packed;

    while (inPtr < endPtr  &&  outBytes <= inLength) {
        // Find the run length value:
        uint rle = 1;
        byte value = *inPtr;
        const byte * pb = inPtr + 1;
        while (pb < endPtr  &&  *pb == value) {
            rle++;
            pb++;
        }

        if (rle >= MIN_RLE_LENGTH) {
            // Run length is long enough to be worth encoding as a run:
            ENCODE_CONTROL_BITS (CODE_RunLength);
            inPtr += rle;
            *outPtr++ = value;
            if (rle > 255) {   // Longer run length:
                *outPtr++ = 0;
                *outPtr++ = (rle >> 24) & 255;
                *outPtr++ = (rle >> 16) & 255;
                *outPtr++ = (rle >>  8) & 255;
                *outPtr++ = rle & 255;
                outBytes += 6;
            } else {
                *outPtr++ = rle;
                outBytes += 2;
            }
        } else if (value == 0) {
            // Zero-valued literal: coded in two bits.
            ENCODE_CONTROL_BITS (CODE_ZeroLiteral);
            inPtr++;
        } else if (value == prevLiteral) {
            // Nonzero literal, same as previous: coded in two bits.
            ENCODE_CONTROL_BITS (CODE_PrevLiteral);
            inPtr++;
        } else {
            // Nonzero literal, different to previous one: coded in 10 bits.
            ENCODE_CONTROL_BITS (CODE_NewLiteral);
            inPtr++;
            prevLiteral = value;
            *outPtr++ = value;
            outBytes++;
        }
    }

    // Flush the control bits:
    controlData >>= controlBits;
    *controlPtr = controlData;

    // Switch to regular copying if necessary:
    if (outBytes > inLength) {
        outBuffer[0] = FLAG_Copied;
        std::memcpy (outBuffer + 1, inBuffer, inLength);
        return (inLength + 1);
    }

#ifdef COMPRESSION_STATS
    printf ("Runs:%u  ZeroLits:%u  PrevLits:%u  DiffLits: %u\n",
            stats[CODE_RunLength], stats[CODE_ZeroLiteral],
            stats[CODE_PrevLiteral], stats[CODE_NewLiteral]);
    printf ("RLE: %u -> %u (%.2f%%)\n", inLength, outBytes,
            (float)outBytes * 100.0 / inLength);
#endif

    return outBytes;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// unpackBytemap():
//      Decompresses the contents of inBuffer to outBuffer using the
//      compression algorithm of packBytemap().
//
//      The input AND output buffer lengths are provided, so the caller
//      must know the lengths from a earlier call to packBytemap().
//      The lengths are used to check for corruption.
//
//      Returns OK on success, or ERROR_Corrupt if any sort of
//      corruption in the compressed data is detected.
//
static errorT
unpackBytemap (const byte * inBuffer, byte * outBuffer, uint inLength,
               uint outLength)
{
    ASSERT (inBuffer != NULL  &&  outBuffer != NULL);
    if (inLength == 0) { return ERROR_Corrupt; }

    // Check if the buffer was copied without compression:

    if (inBuffer[0] == FLAG_Copied) {
        // outLength MUST be one shorter than inLength:
        if (outLength + 1 != inLength) { return ERROR_Corrupt; }
        std::memcpy (outBuffer, inBuffer + 1, outLength);
        return OK;
    }
    if (inBuffer[0] != FLAG_Packed) { return ERROR_Corrupt; }

    const byte * inPtr = inBuffer + 1;
    int inBytesLeft = inLength - 1;
    byte * outPtr = outBuffer;
    int outBytesLeft = outLength;
    uint controlData = *inPtr++;
    uint controlBits = 8;
    inBytesLeft--;
    byte prevLiteral = 0;

    while (outBytesLeft > 0) {
        byte value;
        uint length;
        // Read the two control bits for this literal or run length:
        uint code = controlData & 3;
        controlData >>= 2;
        controlBits -= 2;
        if (controlBits == 0) {
            inBytesLeft--;
            if (inBytesLeft < 0) { return ERROR_Corrupt; }
            controlData = *inPtr++;
            controlBits = 8;
        }

        switch (code) {
        case CODE_ZeroLiteral:      // Literal value zero:
            *outPtr++ = 0;
            outBytesLeft--;
            break;

        case CODE_PrevLiteral:      // Nonzero literal same as previous:
            *outPtr++ = prevLiteral;
            outBytesLeft--;
            break;

        case CODE_RunLength:        // Run length encoding:
            inBytesLeft -= 2;
            if (inBytesLeft < 0) { return ERROR_Corrupt; }
            value = *inPtr++;
            length = *inPtr++;
            if (length == 0) {
                // Longer run length, coded in next 4 bytes:
                inBytesLeft -= 4;
                if (inBytesLeft < 0) { return ERROR_Corrupt; }
                length = *inPtr++;
                length <<= 8; length |= *inPtr++;
                length <<= 8; length |= *inPtr++;
                length <<= 8; length |= *inPtr++;
            }
            outBytesLeft -= length;
            if (outBytesLeft < 0) { return ERROR_Corrupt; }
            while (length--) {
                *outPtr++ = value;
            }
            break;

        case CODE_NewLiteral:       // Nonzero literal with different value:
            prevLiteral = *inPtr++;
            inBytesLeft--;
            *outPtr++ = prevLiteral;
            outBytesLeft--;
            break;

        default:    // UNREACHABLE!
            ASSERT(0);
            return ERROR_Corrupt;
        }
    }

    // Check the buffer lengths for corruption:
    if (inBytesLeft != 0  ||  outBytesLeft != 0) {
        return ERROR_Corrupt;
    }
    return OK;
}

//////////////////////////////////////////////////////////////////////
//  EOF: filter.cpp
//////////////////////////////////////////////////////////////////////
