//////////////////////////////////////////////////////////////////////
//
//  FILE:       mtb.h
//              Memory tablebases
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2003 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


// This file provides classes for compressed in-memory tablebases.
// A memory tablebase (MTB) differs from a regular TB in several ways:
//
//   (1) Its data is contained within the program, not read from a file.
//
//   (2) It may only contain data for a selected portion of an endgame
//       material configuration, with less useful parts excluded to
//       reduce memory requirements.
//
//   (3) Each result may be stored in 1, 2, 4, or 8 bits. A memory
//       tablebase with 4- or 8-bit results will store a distance to
//       mate, while 1- and 2-bit tablebases only store a flag indicating
//       the theoretical result (win/draw/loss).
//
//   (4) Each block of 64*64=4096 results is stored as static compressed
//       data, and decompressed when needed. Decompression is very fast,
//       since a simple byte-oriented sliding window algorithm is used.
//
//   (5) Indexing is not standard across all tablebases; each material
//       configuration uses an index that suits it best. This increases
//       the number of stored broken positions, but keeps things simple.
//
//   (6) For each broken position in an MTB, the value is undefined.
//       This allows the value for each broken position to be set to any
//       value that will minimise compression. For most MTBs, each broken
//       position value is set to be the same as the previous value in
//       the tablebase.


#ifndef SCID_MTB_H
#define SCID_MTB_H

#include "bytepack.h"
#include "../hash.h"
#include "../misc.h"

static BytePacker * bytePacker = new BytePacker();


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ResultGrid
//   A ResultGrid contains 64*64=4096 chess results using 1, 2, 4 or 8
//   bits per result. It can be stored in a compressed format and
//   uncompressed when required.
//
class ResultGrid
{
  private:

    uint BitsPerResult; // Should be 1, 2, or 8 bits per result
    static const uint NumPieces = 2;
    static const uint NumResults = 64 * 64;
    uint NumResultBytes;
    bool HasBeenPacked; // True if the ResultGrid has been compressed
    byte * Data;        // = NumResults * BitsPerResult bits.
    byte * PackedData;
    uint PackedDataLength;
    uint TotalResults;
    uint TotalUnpackedBytes;
    uint TotalPackedBytes;

    bool isValidBitsPerResult (uint bits) {
        return (bits == 1  ||  bits == 2  ||  bits == 4  ||  bits == 8);
    }

  public:
    ResultGrid (uint bitsPerResult)
    {
        ASSERT (isValidBitsPerResult(bitsPerResult));
        BitsPerResult = bitsPerResult;
        HasBeenPacked = false;
        NumResultBytes = NumResults * BitsPerResult / 8;
        Data = new byte[NumResultBytes];
        PackedData = NULL;
        ClearStats();
    }

    ResultGrid (uint bitsPerResult, const byte * packedData, uint packedDataLength)
    {
        ASSERT (isValidBitsPerResult(bitsPerResult));
        BitsPerResult = bitsPerResult;
        HasBeenPacked = false;
        NumResultBytes = NumResults * BitsPerResult / 8;
        Data = NULL;
        PackedData = (byte *) packedData;
        PackedDataLength = packedDataLength;
        ClearStats();
    }

    ~ResultGrid()
    {
        if (Data != NULL) { delete[] Data; }
        if (HasBeenPacked) { delete[] PackedData; }
    }

    uint GetBitsPerResult() { return BitsPerResult; }
    uint GetPackedDataLength() { return PackedDataLength; }
    const byte * GetPackedData() { return PackedData; }

    void Clear()
    {
        for (uint i=0; i < NumResultBytes; i++) {
            Data[i] = 0xFF;
        }
        if (HasBeenPacked) {
#ifdef WINCE
        my_Tcl_Free((char*) PackedData);
#else
        delete[] PackedData;
#endif
            PackedData = NULL;
            PackedDataLength = 0;
        }
    }

    uint ResultCount (uint result)
    {
        uint resultCount = 0;
        for (uint i=0; i < NumResults; i++) {
            if (GetResult(i) == result) {
                resultCount++;
            }
        }
        return resultCount;
    }

    void SetResult (uint result, uint sq1, uint sq2)
    {
        SetResult (result, 64 * sq1 + sq2);
    }

    void SetResult (uint result, uint index)
    {
        ASSERT(result < (1 << BitsPerResult));
        ASSERT(index < NumResults);

        if (BitsPerResult == 1) {
            uint byteIndex = index >> 3;
            uint bitIndex = index & 7;
            byte value = Data[byteIndex];
            value &= (0xFF - (1 << bitIndex));
            value |= (result << bitIndex);
            Data[byteIndex] = value;
        } else if (BitsPerResult == 2) {
            uint byteIndex = index >> 2;
            uint bitIndex = (index & 3) << 1;
            byte value = Data[byteIndex];
            value &= (0xFF - (3 << bitIndex));
            value |= (result << bitIndex);
            Data[byteIndex] = value;
        } else if (BitsPerResult == 4) {
            uint byteIndex = index >> 1;
            uint bitIndex = (index & 1) << 2;
            byte value = Data[byteIndex];
            value &= (0xFF - (15 << bitIndex));
            value |= (result << bitIndex);
            Data[byteIndex] = value;
        } else if (BitsPerResult == 8) {
            Data[index] = result;
        } else {
            ASSERT(0); // Should not happen!
        }

        ASSERT(result == GetResult(index));
    }

    uint GetResult (uint sq1, uint sq2)
    {
        return GetResult (64 * sq1 + sq2);
    }

    uint GetResult (uint index) {
        ASSERT(index < NumResults);
        if (Data == NULL) { Unpack(); }
        uint result = 0;
        if (BitsPerResult == 1) {
            uint byteIndex = index >> 3;
            uint bitIndex = index & 7;
            byte value = Data[byteIndex];
            result = (value >> bitIndex) & 1;
        } else if (BitsPerResult == 2) {
            uint byteIndex = index >> 2;
            uint bitIndex = (index & 3) << 1;
            byte value = Data[byteIndex];
            result = (value >> bitIndex) & 3;
        } else if (BitsPerResult == 4) {
            uint byteIndex = index >> 1;
            uint bitIndex = (index & 1) << 2;
            byte value = Data[byteIndex];
            result = (value >> bitIndex) & 15;
        } else if (BitsPerResult == 8) {
            result = Data[index];
        } else {
            ASSERT(0);
        }
        return result;
    }

    bool Equals (ResultGrid * rg)
    {
        if (BitsPerResult != rg->GetBitsPerResult()) { return false; }
        for (uint i=0; i < NumResults; i++) {
            if (GetResult(i) != rg->GetResult(i)) {
                printf("Result %u: %u != %u\n", i, GetResult(i), rg->GetResult(i));
                return false;
            }
        }
        return true;
    }

    bool Verify()
    {
        Pack();
        ResultGrid * tempGrid =
            new ResultGrid(BitsPerResult, PackedData, PackedDataLength);
        bool isVerified = Equals(tempGrid);
        delete tempGrid;
        return isVerified;
    }

    // Compress a ResultGrid
    uint Pack() {
        if (PackedData != NULL) { return PackedDataLength; }
#ifdef WINCE
        byte * cblock = (byte*) my_Tcl_Alloc(sizeof( byte[NumResultBytes + BytePacker::OverflowBytes]));
        uint csize = bytePacker->Pack(Data, cblock, NumResultBytes);
        PackedData = (byte*) my_Tcl_Alloc(sizeof(byte[csize]));
#else
        byte * cblock = new byte[NumResultBytes + BytePacker::OverflowBytes];
        uint csize = bytePacker->Pack(Data, cblock, NumResultBytes);
        PackedData = new byte[csize];
#endif
        PackedDataLength = csize;
        for (uint i=0; i < csize; i++) {
            PackedData[i] = cblock[i];
        }
        HasBeenPacked = true;
        return csize;
    }

    // Decompress a ResultGrid that was constructed from compressed data
    uint Unpack()
    {
        if (Data != NULL) { return 0; }
#ifdef WINCE
        Data = (byte*) my_Tcl_Alloc(sizeof( byte[NumResultBytes]));
#else
        Data = new byte[NumResultBytes];
#endif
        if (bytePacker->Unpack(PackedData, Data, PackedDataLength, NumResultBytes) != OK) {
            fprintf(stderr, "Unpacking error\n");
            exit(1);
        }
        return 0;
    }


    void ClearStats (void)
    {
        TotalResults = 0;
        TotalPackedBytes = 0;
        TotalUnpackedBytes = 0;
    }

    void UpdateStats (void)
    {
        TotalResults += NumResults;
        TotalUnpackedBytes += NumResultBytes;
        TotalPackedBytes += PackedDataLength;
    }

    void PrintStats (FILE * fp, const char * title)
    {
        if (title != NULL) {
            fprintf (fp, "%-6s ", title);
        }
        fprintf (fp, "%7u * %u-bit, %7u bytes -> %6u bytes, %5.2f%%, %4.2f bits/result\n",
                 TotalResults, BitsPerResult, TotalUnpackedBytes, TotalPackedBytes,
                 (double)TotalPackedBytes * 100.0 / (double)TotalUnpackedBytes,
                 (double)TotalPackedBytes * 8.0 / (double)TotalResults);
    }
};

//////////////////////////////////////////////////////////////////////

struct mtbEntryT {
    uint hash;
    ResultGrid * grid;
    mtbEntryT * next;
};

typedef mtbEntryT * mtbEntryPtr;

class MTB
{
  private:
    const char * Name;
    const byte * PackedData;
    uint BitsPerResult;
    uint CapacityBits;
    uint Capacity;
    uint BucketMask;
    mtbEntryT ** Table;

  public:
    MTB (const char * name, uint bitsPerResult, uint minCapacity)
    {
        Name = strDuplicate(name);
        BitsPerResult = bitsPerResult;
        CapacityBits = 1;
        while (true) {
            Capacity = 1 << CapacityBits;
            if (Capacity >= minCapacity) { break; }
            CapacityBits++;
        }
        BucketMask = Capacity - 1;
        Table = new mtbEntryPtr [Capacity];
        for (uint i=0; i < Capacity; i++) {
            Table[i] = NULL;
        }
    }

    ~MTB()
    {
        delete Name;
        delete[] Table;
    }

    const char * GetName()
    {
        return Name;
    }

    void SetPackedData (const byte * packedData)
    {
        PackedData = packedData;
    }

    uint Hash (squareT sq1, colorT stm)
    {
        return goodHashValues[sq1] ^ stm;
    }
    uint Hash (squareT sq1, squareT sq2, colorT stm)
    {
        return goodHashValues[sq1] ^ goodHashValues[sq2+64] ^ stm;
    }
    uint Hash (squareT sq1, squareT sq2, squareT sq3, colorT stm)
    {
        return goodHashValues[sq1] ^ goodHashValues[sq2+64]  ^
               goodHashValues[sq3+128] ^ stm;
    }

    void Add (squareT sq1, colorT stm, uint packedLength)
    {
        Add (Hash(sq1,stm), packedLength);
    }

    void Add (squareT sq1, squareT sq2, colorT stm, uint packedLength)
    {
        Add (Hash(sq1,sq2,stm), packedLength);
    }

    void Add (squareT sq1, squareT sq2, squareT sq3, colorT stm,
              uint packedLength)
    {
        Add (Hash(sq1,sq2,sq3,stm), packedLength);
    }

    void Add (uint hash, uint packedDataLength)
    {
        if (Find(hash) != NULL) {
            printf("Hash collision in MTB: %s\n", Name);
            return;
        }
        ResultGrid * grid =
            new ResultGrid (BitsPerResult, PackedData, packedDataLength);
        // To unpack all compressed memory tablebases at startup,
        // uncomment the grid->Unpack() call below.
        // grid->Unpack();
        PackedData += packedDataLength;
        uint bucket = hash & BucketMask;
#ifdef WINCE
        mtbEntryT * entry = (mtbEntryT * ) my_Tcl_Alloc(sizeof( mtbEntryT));
#else
        mtbEntryT * entry = new mtbEntryT;
#endif
        entry->hash = hash;
        entry->grid = grid;
        entry->next = Table[bucket];
        Table[bucket] = entry;
    }

    ResultGrid * Find (squareT sq1, colorT stm)
    {
        return Find (Hash(sq1,stm));
    }

    ResultGrid * Find (squareT sq1, squareT sq2, colorT stm)
    {
        return Find (Hash(sq1,sq2,stm));
    }

    ResultGrid * Find (squareT sq1, squareT sq2, squareT sq3, colorT stm)
    {
        return Find (Hash(sq1,sq2,sq3,stm));
    }

    ResultGrid * Find (uint hash)
    {
        uint bucket = hash & BucketMask;
        mtbEntryT * entry = Table[bucket];
        while (entry != NULL) {
            if (entry->hash == hash) { return entry->grid; }
            entry = entry->next;
        }
        return NULL;
    }

    uint LongestChainLength()
    {
        uint longest = 0;
        for (uint i=0; i < Capacity; i++) {
            uint length = 0;
            mtbEntryT * entry = Table[i];
            while (entry != NULL) {
                length++;
                entry = entry->next;
            }
            if (length > longest) { longest = length; }
        }
        return longest;
    }

};


#endif // SCID_MTB_H

//////////////////////////////////////////////////////////////////////
//  EOF: mtb.h
//////////////////////////////////////////////////////////////////////
