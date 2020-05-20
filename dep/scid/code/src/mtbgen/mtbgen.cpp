//////////////////////////////////////////////////////////////////////
//
//  FILE:       mtbgen.cpp
//              Generate memory tablebases for the Scid chess engine.
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2003 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


// This program reads Nalimov-format tablebases to generate selected
// compressed in-memory tablebases used by the Scidlet chess program.
// It produces the file mtbdata.h. See mtb.h and recog.cpp for details.

#include <stdio.h>
#include <stdlib.h>
#include "../common.h"
#include "../error.h"
#include "mtb.h"
#include "../position.h"
#include "../probe.h"

static const char * SLASHES =
    "//////////////////////////////////////////////////////////////////////";

static inline uint PAIR (uint x, uint y)
{
    return x | (y << 8);
}

static inline uint TRIPLET (uint x, uint y, uint z)
{
    return x | (y << 8) | (z << 16);
}

static inline squareT FIRST (uint pair)
{
    return pair & 255;
}

static inline squareT SECOND (uint pair)
{
    return (pair >> 8) & 255;
}

static inline squareT THIRD (uint triplet)
{
    return (triplet >> 16) & 255;
}


void tbFailure(const char * material, Position * pos)
{
    colorT stm = pos->GetToMove();
    fprintf (stderr, "%s %ctm: tablebase access failed\n",
             material, color_Char(stm));
    pos->DumpBoard (stderr);
    exit (1);
}

inline bool uniqueSquares (squareT sq1, squareT sq2, squareT sq3)
{
    return (sq1 != sq2  &&  sq1 != sq3  &&  sq2 != sq3);
}

inline bool uniqueSquares (squareT sq1, squareT sq2, squareT sq3, squareT sq4)
{
    return (sq1 != sq2  &&  sq1 != sq3  &&  sq1 != sq4
                        &&  sq2 != sq3  &&  sq2 != sq4
                                        &&  sq3 != sq4);
}

inline bool uniqueSquares (squareT sq1, squareT sq2, squareT sq3,
                           squareT sq4, squareT sq5)
{
    return (sq1 != sq2  &&  sq1 != sq3  &&  sq1 != sq4  &&  sq1 != sq5
                        &&  sq2 != sq3  &&  sq2 != sq4  &&  sq2 != sq5
                                        &&  sq3 != sq4  &&  sq3 != sq5
                                                        &&  sq4 != sq5);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MTBWriter
//   This class is used to write C++ code to setup a memory tablebase.
//
class MTBWriter
{
  private:

    struct mtbWriterEntryT {
        squareT sq1, sq2, sq3;
        colorT stm;
        uint packedLength;
    };

    static const uint MAX_MTB_ENTRIES = 1024;
    static const uint MAX_PACKED_BYTES = 1 << 18;

    byte * PackedData;
    uint PackedDataLength;
    char * Name;
    uint BitsPerResult;
    uint NumEntries;
    mtbWriterEntryT * Entries[MAX_MTB_ENTRIES];

  public:
#ifdef WINCE
  void* operator new(size_t sz) {
    void* m = my_Tcl_Alloc(sz);
    return m;
  }
  void operator delete(void* m) {
    my_Tcl_Free((char*)m);
  }
  void* operator new [] (size_t sz) {
    void* m = my_Tcl_AttemptAlloc(sz);
    return m;
  }

  void operator delete [] (void* m) {
    my_Tcl_Free((char*)m);
  }

#endif
    MTBWriter (const char * name, uint bitsPerResult)
    {
        PackedData = new byte[MAX_PACKED_BYTES];
        PackedDataLength = 0;
        Name = strDuplicate(name);
        BitsPerResult = bitsPerResult;
        NumEntries = 0;
    }

    ~MTBWriter ()
    {
        delete[] PackedData;
        delete[] Name;
    }

    void Add (ResultGrid * grid, squareT sq, colorT stm) {
        Add (grid, sq, NS, NS, stm);
    }

    void Add (ResultGrid * grid, squareT sq1, squareT sq2, colorT stm) {
        Add (grid, sq1, sq2, NS, stm);
    }

    void Add (ResultGrid * grid, squareT sq1, squareT sq2, squareT sq3,
              colorT stm);

    void Write (FILE * fp);
};

void
MTBWriter::Add (ResultGrid * grid, squareT sq1, squareT sq2, squareT sq3,
                 colorT stm)
{
    ASSERT (BitsPerResult == grid->GetBitsPerResult())
    mtbWriterEntryT * entry = new mtbWriterEntryT;
    if (NumEntries >= MAX_MTB_ENTRIES) {
        fprintf (stderr, "Full MTBWriter: %s\n", Name);
        exit(1);
    }
    Entries[NumEntries++] = entry;
    entry->sq1 = sq1;
    entry->sq2 = sq2;
    entry->sq3 = sq3;
    entry->stm = stm;
    grid->Pack();
    if (! grid->Verify()) {
        fprintf (stderr, "%s data verify error\n", Name);
        exit(1);
    }
    entry->packedLength = grid->GetPackedDataLength();
    if (PackedDataLength + entry->packedLength > MAX_PACKED_BYTES) {
        fprintf (stderr, "Full MTBWriter: %s\n", Name);
        exit(1);
    }
    const byte * packedData = grid->GetPackedData();
    for (uint i=0; i < entry->packedLength; i++) {
        PackedData[PackedDataLength++] = *packedData++;
    }
}

void
MTBWriter::Write (FILE * fp)
{
    fprintf (fp, "%s\n", SLASHES);
    fprintf (fp, "//\n");
    fprintf (fp, "// %s\n\n", Name);

    // Write the compressed data as a constant byte array:
    fprintf (fp, "static const byte mtbdata_%s[%u] = {\n    ",
             Name, PackedDataLength);
    uint i, width = 4;
    for (i=0; i < PackedDataLength; i++) {
        byte result = PackedData[i];
        fprintf (fp, "%u", result);
        width++;
        if (result >= 10) { width++; }
        if (result >= 100) { width++; }
        if (i+1 < PackedDataLength) {
            fprintf (fp, ",");
            width++;
            if (width > 70) {
                fprintf (fp, "\n    ");
                width = 4;
            }
        }
    }
    fprintf (fp, "\n};\n\n");
    fprintf (fp, "static MTB * mtb_%s = NULL;\n\n", Name);

    // Write the MTB initialization function:
    fprintf (fp, "void initMTB_%s()\n{\n", Name);
    fprintf (fp, "    mtb_%s = new MTB (\"%s\", %u, %u);\n",
             Name, Name, BitsPerResult, NumEntries);
    fprintf (fp, "    mtb_%s->SetPackedData (mtbdata_%s);\n", Name, Name);
    for (i=0; i < NumEntries; i++) {
        mtbWriterEntryT * entry = Entries[i];
        fprintf (fp, "    mtb_%s->Add (", Name);
        squareT sq1 = entry->sq1;
        squareT sq2 = entry->sq2;
        squareT sq3 = entry->sq3;
        colorT stm = entry->stm;
        if (sq1 != NS) {
            fprintf (fp, "%c%c, ",
                     toupper(square_FyleChar(sq1)), square_RankChar(sq1));
        }
        if (sq2 != NS) {
            fprintf (fp, "%c%c, ",
                     toupper(square_FyleChar(sq2)), square_RankChar(sq2));
        }
        if (sq3 != NS) {
            fprintf (fp, "%c%c, ",
                     toupper(square_FyleChar(sq3)), square_RankChar(sq3));
        }
        fprintf (fp, "%s, %u);\n", stm == WHITE ? "WHITE" : "BLACK",
                 entry->packedLength);
    }
    fprintf (fp, "}\n\n");
}


//////////////////////////////////////////////////////////////////////

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// kqk
//   Stores every KQ-K result for White to move, 4 bits per result.
//
void kqk (FILE * fp)
{
    const uint bitsPerResult = 4;
    MTBWriter * writer = new MTBWriter("KQK", bitsPerResult);
    Position * pos = new Position();
    ResultGrid * resultGrid = new ResultGrid(bitsPerResult);

    // Black king squares:
    squareT kingSquares[] = {
        A1, B1, C1, D1, B2, C2, D2, C3, D3, D4,
        NULL_SQUARE
    };

    for (colorT stm = WHITE; stm <= WHITE; stm++) {
        squareT * bkSquares = kingSquares;
        while (*bkSquares != NULL_SQUARE) {
            resultGrid->Clear();
            squareT bk = *bkSquares;
            uint prevres = 0;
            for (squareT wk=A1; wk <= H8; wk++) {
                for (squareT wq=A1; wq <= H8; wq++) {
                    uint result = prevres;  // Broken: just use previous result
                    if (uniqueSquares (wk, bk, wq)) {
                        pos->Clear();
                        pos->AddPiece(WK, wk);
                        pos->AddPiece(BK, bk);
                        pos->AddPiece(WQ, wq);
                        pos->SetToMove(stm);
                        if (pos->IsLegal()) {
                            int score = 0;
                            if (scid_TB_Probe (pos, &score) == OK) {
                                result = ((score < 0) ? -score : score);
                                prevres=result;
                            } else {
                                tbFailure("KQK", pos);
                            }
                        }
                    }
                    resultGrid->SetResult (result, wk, wq);
                }
            }
            writer->Add(resultGrid, bk, stm);
            resultGrid->UpdateStats ();
            bkSquares++;
        }
    }
    writer->Write(fp);
    resultGrid->PrintStats (stdout, "KQ-K");
    delete pos;
    delete resultGrid;
    delete writer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// krk
//   Stores every KR-K result for White to move, 4 bits per result.
//
void krk (FILE * fp)
{
    const uint bitsPerResult = 4;
    MTBWriter * writer = new MTBWriter("KRK", bitsPerResult);
    Position * pos = new Position();
    ResultGrid * resultGrid = new ResultGrid(bitsPerResult);

    squareT kingSquares[] = {
        A1, B1, C1, D1, B2, C2, D2, C3, D3, D4,
        NULL_SQUARE
    };
    for (colorT stm = WHITE; stm <= WHITE; stm++) {
        squareT * bkSquares = kingSquares;
        while (*bkSquares != NULL_SQUARE) {
            resultGrid->Clear();
            squareT bk = *bkSquares;
            uint prevResult = 0;
            for (squareT wk=A1; wk <= H8; wk++) {
                for (squareT wr=A1; wr <= H8; wr++) {
                    uint result = prevResult; // Broken: repeat previous result
                    if (uniqueSquares (wk, bk, wr)) {
                        pos->Clear();
                        pos->AddPiece(WK, wk);
                        pos->AddPiece(BK, bk);
                        pos->AddPiece(WR, wr);
                        pos->SetToMove(stm);
                        if (pos->IsLegal()) {
                            int score = 0;
                            if (scid_TB_Probe (pos, &score) == OK) {
                                result = ((score < 0) ? -score : score-1);
                                prevResult = result;
                            } else {
                                tbFailure("KRK", pos);
                            }
                        }
                    }
                    // Add the result to the table:
                    resultGrid->SetResult (result, wk, wr);
                    prevResult = result;
                }
            }
            writer->Add(resultGrid, bk, stm);
            resultGrid->UpdateStats ();
            bkSquares++;
        }
    }
    writer->Write (fp);
    resultGrid->PrintStats (stdout, "KR-K");
    delete pos;
    delete resultGrid;
    delete writer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// kpk
//   Stores every KP-K result for White or Black to move, 1 bit per result.
//
void kpk (FILE * fp)
{
    const uint bitsPerResult = 1;
    MTBWriter * writer = new MTBWriter ("KPK", bitsPerResult);
    Position * pos = new Position();
    ResultGrid * resultGrid = new ResultGrid(bitsPerResult);

    squareT pawnSquares[] = {
        A2, B2, C2, D2, A3, B3, C3, D3, A4, B4, C4, D4,
        A5, B5, C5, D5, A6, B6, C6, D6, A7, B7, C7, D7,
        NULL_SQUARE
    };

    for (colorT stm = WHITE; stm <= BLACK; stm++) {
        squareT * wpSquares = pawnSquares;
        while (*wpSquares != NULL_SQUARE) {
            resultGrid->Clear();
            squareT wp = *wpSquares;
            for (squareT wk=A1; wk <= H8; wk++) {
                for (squareT bk=A1; bk <= H8; bk++) {
                    uint result = 1;  // Broken: use same value as win
                    if (uniqueSquares (wk, bk, wp)) {
                        pos->Clear();
                        pos->AddPiece(WK, wk);
                        pos->AddPiece(BK, bk);
                        pos->AddPiece(WP, wp);
                        pos->SetToMove(stm);
                        if (pos->IsLegal()) {
                            int score = 0;
                            if (scid_TB_Probe (pos, &score) == OK) {
                                result = ((score == 0) ? 0 : 1);
                            } else {
                                tbFailure("KPK", pos);
                            }
                        }
                    }
                    // Add the result to the table:
                    resultGrid->SetResult (result, bk, wk);
                }
            }
            writer->Add (resultGrid, wp, stm);
            resultGrid->UpdateStats ();
            wpSquares++;
        }
    }
    writer->Write (fp);
    resultGrid->PrintStats (stdout, "KP-K");
    delete resultGrid;
    delete pos;
    delete writer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// kqkq
//   Selected KQ-KQ results.
//
void kqkq (FILE * fp)
{
    const uint bitsPerResult = 2;
    MTBWriter * writer = new MTBWriter ("KQKQ", bitsPerResult);
    Position * pos = new Position();
    ResultGrid * resultGrid = new ResultGrid (bitsPerResult);

    // Most KQ-KQ positions are draws or trivial wins that capture the
    // opposing queen (directly or by a skewer), but there are some mate
    // threat tactics which can take time to find. Since KQKQ often
    // occurs in analysis of pawn endings, we use memory tablebases
    // for selected king configurations where mate threats are possible.

    // KING_SQUARES: array of PAIR(wkSquare,bkSquare) for WK and BK
    uint KING_SQUARES[] = {
        // Black king on a1
        PAIR(C2,A1), PAIR(C3,A1),
        // Black king on b1
        PAIR(A3,B1), PAIR(B3,B1), PAIR(C3,B1), PAIR(D2,B1), PAIR(D3,B1),
        // Black king on c1
        PAIR(C3,C1), PAIR(D3,C1), PAIR(E2,C1),
        // Black king on b2
        PAIR(D2,B2), PAIR(D3,B2), PAIR(D4,B2),
        // Black king on c2
        PAIR(C4,C2), PAIR(D4,C2), PAIR(E2,C2), PAIR(E3,C2),
        PAIR(NULL_SQUARE,NULL_SQUARE)
    };

    // We only need WTM for KQKQ because material is symmetrical.

    uint * kingSquares = KING_SQUARES;
    while (*kingSquares != PAIR(NULL_SQUARE,NULL_SQUARE)) {
        resultGrid->Clear();
        squareT wk = FIRST(*kingSquares);
        squareT bk = SECOND(*kingSquares);
        for (squareT wq = A1; wq <= H8; wq++) {
            for (squareT bq = A1; bq <= H8; bq++) {
                uint result = 1; // Broken: same value as White (STM) win
                if (uniqueSquares (wk, bk, wq, bq)) {
                    pos->Clear();
                    pos->AddPiece(WK, wk);
                    pos->AddPiece(BK, bk);
                    pos->AddPiece(WQ, wq);
                    pos->AddPiece(BQ, bq);
                    pos->SetToMove(WHITE);
                    if (pos->IsLegal()) {
                        int score = 0;
                        if (scid_TB_Probe (pos, &score) == OK) {
                            if (score == 0) { result = 0; } // Draw
                            else if (score > 0) { result = 1; }  // STM wins
                            else {result = 2; } // STM loses
                        } else {
                            tbFailure("KQKQ", pos);
                        }
                    }
                }
                // Add the result to the table:
                resultGrid->SetResult (result, wq, bq);
            }
        }
        writer->Add (resultGrid, wk, bk, WHITE);
        resultGrid->UpdateStats ();
        kingSquares++;
    }
    writer->Write (fp);
    resultGrid->PrintStats (stdout, "KQ-KQ");
    delete resultGrid;
    delete pos;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// kqkp
//   Selected KQ-KP results for White to move with the black pawn one
//   square away from queening, one bit per result.
//   Note that there are a few rare KQ-KP WTM positions where Black wins;
//   these are checked in the recognizer code.
//
void kqkp (FILE * fp)
{
    const uint bitsPerResult = 1;
    MTBWriter * writer = new MTBWriter ("KQKP", bitsPerResult);
    Position * pos = new Position();
    ResultGrid * resultGrid = new ResultGrid (bitsPerResult);

    // BLACK_SQUARES: array of PAIR(pawnSquare,kingSquare) for black pieces
    uint BLACK_SQUARES[] = {
        PAIR(A2,A1), PAIR(A2,B1), PAIR(A2,C1),
                     PAIR(A2,B2), PAIR(A2,C2),

        PAIR(C2,A1), PAIR(C2,B1), PAIR(C2,C1), PAIR(C2,D1), PAIR(C2,E1),
        PAIR(C2,A2), PAIR(C2,B2),              PAIR(C2,D2), PAIR(C2,E2),
        PAIR(C2,A3), PAIR(C2,B3), PAIR(C2,C3), PAIR(C2,D3), PAIR(C2,E3),

        PAIR(D2,C1), PAIR(D2,E1), PAIR(D2,C2), PAIR(D2,E2),

        // TODO: BP on c3 as well? it has some draws...

        PAIR(NULL_SQUARE,NULL_SQUARE)
    };

    for (colorT stm = WHITE; stm <= WHITE; stm++) {
        uint * blackSquares = BLACK_SQUARES;
        while (*blackSquares != PAIR(NULL_SQUARE,NULL_SQUARE)) {
            resultGrid->Clear();
            uint prevResult = 1;  // Default value for initial broken result
            squareT bp = FIRST(*blackSquares);
            squareT bk = SECOND(*blackSquares);
            for (squareT wk = A1; wk <= H8; wk++) {
                for (squareT wq = A1; wq <= H8; wq++) {
                    uint result = prevResult;  // Broken: use previous result
                    if (uniqueSquares (wk, bk, wq, bp)) {
                        pos->Clear();
                        pos->AddPiece(WK, wk);
                        pos->AddPiece(BK, bk);
                        pos->AddPiece(WQ, wq);
                        pos->AddPiece(BP, bp);
                        pos->SetToMove(stm);
                        if (pos->IsLegal()) {
                            int score = 0;
                            if (scid_TB_Probe (pos, &score) == OK) {
                                if (stm == BLACK) { score = -score; }
                                if (score < 0) {
                                    printf ("Warning: WTM loss found for KQKP\n");
                                    pos->DumpBoard(stdout);
                                }
                                result = (score == 0 ? 0 : 1);
                                prevResult = result;
                            } else {
                                tbFailure("KQKP", pos);
                            }
                        }
                    }
                    // Add the result to the table:
                    resultGrid->SetResult (result, wk, wq);
                }
            }
            writer->Add (resultGrid, bp, bk, stm);
            resultGrid->UpdateStats ();
            blackSquares++;
        }
    }
    writer->Write (fp);
    resultGrid->PrintStats (stdout, "KQ-KP");
    delete resultGrid;
    delete pos;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// krkp
//   Selected KR-KP results, White to move.
//
void krkp (FILE * fp)
{
    const uint bitsPerResult = 2;
    MTBWriter * writer = new MTBWriter ("KRKP", bitsPerResult);
    Position * pos = new Position();
    ResultGrid * resultGrid = new ResultGrid (bitsPerResult);

    // BLACK_SQUARES: array of PAIR(pawnSquare,kingSquare) for black pieces
    uint BLACK_SQUARES[] = {
        PAIR(A2,A1), PAIR(A2,B1), PAIR(A2,C1),
                     PAIR(A2,B2), PAIR(A2,C2),
        PAIR(A2,A3), PAIR(A2,B3), PAIR(A2,C3),

        PAIR(B2,A1), PAIR(B2,B1), PAIR(B2,C1), PAIR(B2,D1),
        PAIR(B2,A2),              PAIR(B2,C2), PAIR(B2,D2),
        PAIR(B2,A3), PAIR(B2,B3), PAIR(B2,C3), PAIR(B2,D3),

        PAIR(C2,A1), PAIR(C2,B1), PAIR(C2,C1), PAIR(C2,D1), PAIR(C2,E1),
        PAIR(C2,A2), PAIR(C2,B2),              PAIR(C2,D2), PAIR(C2,E2),
        PAIR(C2,A3), PAIR(C2,B3), PAIR(C2,C3), PAIR(C2,D3), PAIR(C2,E3),

        PAIR(D2,B1), PAIR(D2,C1), PAIR(D2,D1), PAIR(D2,E1), PAIR(D2,F1),
        PAIR(D2,B2), PAIR(D2,C2),              PAIR(D2,E2), PAIR(D2,F2),
        PAIR(D2,B3), PAIR(D2,C3), PAIR(D2,D3), PAIR(D2,E3), PAIR(D2,F3),

        PAIR(A3,A2), PAIR(A3,B2), PAIR(A3,B3), PAIR(A3,A4), PAIR(A3,B4),

        PAIR(B3,A2), PAIR(B3,B2), PAIR(B3,C2), PAIR(B3,A3), PAIR(B3,C3),
        PAIR(B3,A4), PAIR(B3,B4), PAIR(B3,C4),

        PAIR(C3,B2), PAIR(C3,C2), PAIR(C3,D2), PAIR(C3,B3), PAIR(C3,D3),
        PAIR(C3,B4), PAIR(C3,C4), PAIR(C3,D4),

        PAIR(D3,C2), PAIR(D3,D2), PAIR(D3,E2), PAIR(D3,C3), PAIR(D3,E3),
        PAIR(D3,C4), PAIR(D3,D4), PAIR(D3,E4),

        PAIR(A4,A3), PAIR(A4,B3), PAIR(A4,B4), PAIR(A4,A5), PAIR(A4,B5),

        PAIR(B4,A3), PAIR(B4,B3), PAIR(B4,C3), PAIR(B4,A4), PAIR(B4,C4),
        PAIR(B4,A5), PAIR(B4,B5), PAIR(B4,C5),

        PAIR(C4,B3), PAIR(C4,C3), PAIR(C4,D3), PAIR(C4,B4), PAIR(C4,D4),
        PAIR(C4,B5), PAIR(C4,C5), PAIR(C4,D5),

        PAIR(D4,C3), PAIR(D4,D3), PAIR(D4,E3), PAIR(D4,C4), PAIR(D4,E4),
        PAIR(D4,C5), PAIR(D4,D5), PAIR(D4,E5),

        PAIR(A5,A4), PAIR(A5,B4), PAIR(A5,B5), PAIR(A5,A6), PAIR(A5,B6),

        PAIR(B5,A4), PAIR(B5,B4), PAIR(B5,C4), PAIR(B5,A5), PAIR(B5,C5),
        PAIR(B5,A6), PAIR(B5,B6), PAIR(B5,C6),

        PAIR(C5,B4), PAIR(C5,C4), PAIR(C5,D4), PAIR(C5,B5), PAIR(C5,D5),
        PAIR(C5,B6), PAIR(C5,C6), PAIR(C5,D6),

        PAIR(D5,C4), PAIR(D5,D4), PAIR(D5,E4), PAIR(D5,C5), PAIR(D5,E5),
        PAIR(D5,C6), PAIR(D5,D6), PAIR(D5,E6),

        PAIR(NULL_SQUARE,NULL_SQUARE)
    };

    for (colorT stm = WHITE; stm <= WHITE; stm++) {
        uint * blackSquares = BLACK_SQUARES;
        while (*blackSquares != PAIR(NULL_SQUARE,NULL_SQUARE)) {
            resultGrid->Clear();
            uint prevResult = 0;  // Default value for previous result
            squareT bp = FIRST(*blackSquares);
            squareT bk = SECOND(*blackSquares);
            for (squareT wk = A1; wk <= H8; wk++) {
                for (squareT wr = A1; wr <= H8; wr++) {
                    uint result = prevResult;  // Broken: use previous result
                    if (uniqueSquares (wk, bk, wr, bp)) {
                        pos->Clear();
                        pos->AddPiece(WK, wk);
                        pos->AddPiece(BK, bk);
                        pos->AddPiece(WR, wr);
                        pos->AddPiece(BP, bp);
                        pos->SetToMove(stm);
                        if (pos->IsLegal()) {
                            int score = 0;
                            if (scid_TB_Probe (pos, &score) == OK) {
                                if (stm == BLACK) { score = -score; }
                                if (score == 0) { result = 0; }
                                else if (score > 0) { result = 1; }
                                else {result = 2; }
                            } else {
                                tbFailure("KRKP", pos);
                            }
                        }
                    }
                    // Add the result to the table:
                    resultGrid->SetResult (result, wk, wr);
                    prevResult = result;
                }
            }
            writer->Add (resultGrid, bp, bk, stm);
            resultGrid->UpdateStats ();
            blackSquares++;
        }
    }
    writer->Write (fp);
    resultGrid->PrintStats (stdout, "KR-KP");
    delete resultGrid;
    delete pos;
    delete writer;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// kpkp
//   Selected KP-KP results.
//
void kpkp (FILE * fp)
{
    const uint bitsPerResult = 2;
    MTBWriter * writer = new MTBWriter ("KPKP", bitsPerResult);
    Position * pos = new Position();
    ResultGrid * resultGrid = new ResultGrid (bitsPerResult);

    // Database searches indicated that position with pawns on the same file
    // occur often, and rammed pawns (where neither pawn can move) are
    // especially common. There are 20 rammed pawn formations (a2a3 ... d6d7).

    // PAWN_SQUARES: array of PAIR(wpSquare,bpSquare) for WP and BP
    uint PAWN_SQUARES[] = {
        PAIR(A2,A3), PAIR(B2,B3), PAIR(C2,C3), PAIR(D2,D3),
        PAIR(A3,A4), PAIR(B3,B4), PAIR(C3,C4), PAIR(D3,D4),
        PAIR(A4,A5), PAIR(B4,B5), PAIR(C4,C5), PAIR(D4,D5),
        PAIR(A5,A6), PAIR(B5,B6), PAIR(C5,C6), PAIR(D5,D6),
        PAIR(A6,A7), PAIR(B6,B7), PAIR(C6,C7), PAIR(D6,D7),

        // TODO: Add more (WP,BP) pairs, but avoid ({x}5,{x+1}5) and
        // ({x}5,{x-1}5), for example (A5,B5) to avoid all possible
        // en passant positions.

        // Pawns on adjacent files, neither passed:
        PAIR(A5,B7), PAIR(A4,B6), PAIR(A3,B5), PAIR(A2,B4),
        PAIR(B5,A7), PAIR(B4,A6), PAIR(B3,A5), PAIR(B2,A4),
        PAIR(B5,C7), PAIR(B4,C6), PAIR(B3,C5), PAIR(B2,C4),

        // Pawns on non-adjacent files, or adjacent but passed:
        PAIR(A5,B4), PAIR(A6,B3), PAIR(A7,B2),
        PAIR(B5,C4), PAIR(B6,C3), PAIR(B7,C2),

        PAIR(NULL_SQUARE,NULL_SQUARE)
    };

    // We only need WTM for KPKP because material is symmetrical.

    uint * pawnSquares = PAWN_SQUARES;
    while (*pawnSquares != PAIR(NULL_SQUARE,NULL_SQUARE)) {
        resultGrid->Clear();
        squareT wp = FIRST(*pawnSquares);
        squareT bp = SECOND(*pawnSquares);
        uint prevResult = 0;  // Default value for a broken result
        for (squareT wk = A1; wk <= H8; wk++) {
            for (squareT bk = A1; bk <= H8; bk++) {
                uint result = prevResult;  // Broken: use previous result
                if (uniqueSquares (wk, bk, wp, bp)) {
                    pos->Clear();
                    pos->AddPiece(WK, wk);
                    pos->AddPiece(BK, bk);
                    pos->AddPiece(WP, wp);
                    pos->AddPiece(BP, bp);
                    pos->SetToMove(WHITE);
                    if (pos->IsLegal()) {
                        int score = 0;
                        if (scid_TB_Probe (pos, &score) == OK) {
                            if (score == 0) { result = 0; } // Draw
                            else if (score > 0) { result = 1; }  // STM wins
                            else {result = 2; } // STM loses
                        } else {
                            tbFailure("KPKP", pos);
                        }
                    }
                }
                // Add the result to the table:
                resultGrid->SetResult (result, wk, bk);
                prevResult = result;
            }
        }
        writer->Add (resultGrid, wp, bp, WHITE);
        resultGrid->UpdateStats ();
        pawnSquares++;
    }
    writer->Write (fp);
    resultGrid->PrintStats (stdout, "KP-KP");
    delete resultGrid;
    delete pos;
    delete writer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// krpkr
//   Selected KRP-KR results, pawn on 6th/7th rank.
//
void krpkr (FILE * fp)
{
    const uint bitsPerResult = 2;
    MTBWriter * writer = new MTBWriter ("KRPKR", bitsPerResult);
    Position * pos = new Position();
    ResultGrid * resultGrid = new ResultGrid(bitsPerResult);

    // PKK_SQUARES: array of TRIPLET(wpSquare,wkSquare,bkSquare)
    uint PKK_SQUARES[] = {
        // White pawn on a7
        TRIPLET(A7,A8,C8), TRIPLET(A7,A8,C7), TRIPLET(A7,A8,D7),
        TRIPLET(A7,A6,A8), TRIPLET(A7,B6,A8),

        // White pawn on b7
        TRIPLET(B7,B8,D7), TRIPLET(B7,B8,D8), TRIPLET(B7,B8,E7),
        TRIPLET(B7,A6,B8), TRIPLET(B7,A6,C7), TRIPLET(B7,B6,B8),
        TRIPLET(B7,C6,B8), TRIPLET(B7,C6,A7), TRIPLET(B7,C6,E7),

        // White pawn on c7
        TRIPLET(C7,C8,A7), TRIPLET(C7,C8,A6), TRIPLET(C7,C8,E7),
        TRIPLET(C7,C8,E6), TRIPLET(C7,C8,F7),

        // White pawn on d7
        TRIPLET(D7,D8,B7), TRIPLET(D7,D8,C6), TRIPLET(D7,D8,E6),
        TRIPLET(D7,D8,F7), TRIPLET(D7,C6,D8),

        // White pawn on a6
        TRIPLET(A6,A7,C6), TRIPLET(A6,A7,C7), TRIPLET(A6,A7,C8),
        TRIPLET(A6,A7,D7), TRIPLET(A6,B6,A8), TRIPLET(A6,B5,A7),

        // White pawn on b6
        TRIPLET(B6,B7,D6), TRIPLET(B6,B7,D7), TRIPLET(B6,B7,D8),
        TRIPLET(B6,B8,A6), TRIPLET(B6,B8,C6),
        TRIPLET(B6,A6,B8), TRIPLET(B6,A6,C8), TRIPLET(B6,C6,B8),

        // White pawn on c6
        TRIPLET(C6,C7,A6), TRIPLET(C6,C7,A7), TRIPLET(C6,C7,E7),
        TRIPLET(C6,B6,C8), TRIPLET(C6,D6,C8),

        // TODO: White pawn on d6

        TRIPLET(NS,NS,NS)
    };

    // KRP-KR is an important practical tablebase, so we store results
    // for both White and Black to move.

    for (colorT stm = WHITE; stm <= BLACK; stm++) {
        uint * pkkSquares = PKK_SQUARES;
        while (*pkkSquares != TRIPLET(NS,NS,NS)) {
            resultGrid->Clear();
            squareT wp = FIRST(*pkkSquares);
            squareT wk = SECOND(*pkkSquares);
            squareT bk = THIRD(*pkkSquares);
            uint prevResult = 0;  // Default value for a broken result

            for (squareT wr = A1; wr <= H8; wr++) {
                for (squareT br = A1; br <= H8; br++) {
                    uint result = prevResult; // Broken: use previous result
                    if (uniqueSquares (wk, bk, wr, br, wp)) {
                        pos->Clear();
                        pos->AddPiece(WK, wk);
                        pos->AddPiece(BK, bk);
                        pos->AddPiece(WR, wr);
                        pos->AddPiece(BR, br);
                        pos->AddPiece(WP, wp);
                        pos->SetToMove(stm);
                        if (pos->IsLegal()) {
                            int score = 0;
                            if (scid_TB_Probe (pos, &score) == OK) {
                                if (score == 0) { result = 0; }
                                else if (score > 0) { result = 1; }
                                else {result = 2; }
                            } else {
                                tbFailure("KRPKR", pos);
                            }
                        }
                    }
                    // Add the result to the table:
                    resultGrid->SetResult (result, wr, br);
                    prevResult = result;
                }
            }
            writer->Add (resultGrid, wp, wk, bk, stm);
            resultGrid->UpdateStats ();
            pkkSquares++;
        }
    }
    writer->Write (fp);
    resultGrid->PrintStats (stdout, "KRP-KR");
    delete resultGrid;
    delete pos;
    delete writer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// kppkr
//   Selected KPP-KR results. We store selected tablebases for White (with
//   the rook) to move against advanced black pawns with their King nearby.
//
void kppkr (FILE * fp)
{
    const uint bitsPerResult = 2;
    MTBWriter * writer = new MTBWriter ("KPPKR", bitsPerResult);
    Position * pos = new Position();
    ResultGrid * resultGrid = new ResultGrid(bitsPerResult);

    // The most common normalized pawn configurations are: a+b pawns,
    // then b+c pawns, then a+c pawns.

    // BLACK_SQUARES: array of TRIPLET(bpSquare,bpSquare,bkSquare)
    uint BLACK_SQUARES[] = {
        // a+b pawns
        TRIPLET(A2,B2,C2),
        TRIPLET(A2,B3,C2), TRIPLET(A2,B3,C3),
        TRIPLET(A3,B2,B3), TRIPLET(A3,B2,C2), TRIPLET(A3,B2,C3),
        // TODO: more a+b pawns

        // TODO: b+c pawns
        // TODO: a+c pawns

        TRIPLET(NS,NS,NS)
    };

    for (colorT stm = WHITE; stm <= WHITE; stm++) {
        uint * blackSquares = BLACK_SQUARES;
        while (*blackSquares != TRIPLET(NS,NS,NS)) {
            resultGrid->Clear();
            squareT bp1 = FIRST(*blackSquares);
            squareT bp2 = SECOND(*blackSquares);
            squareT bk = THIRD(*blackSquares);
            uint prevResult = 0;  // Default value for a broken result

            for (squareT wk = A1; wk <= H8; wk++) {
                for (squareT wr = A1; wr <= H8; wr++) {
                    uint result = prevResult; // Broken: use previous result
                    if (uniqueSquares (wk, bk, wr, bp1, bp2)) {
                        pos->Clear();
                        pos->AddPiece(WK, wk);
                        pos->AddPiece(BK, bk);
                        pos->AddPiece(WR, wr);
                        pos->AddPiece(BP, bp1);
                        pos->AddPiece(BP, bp2);
                        pos->SetToMove(stm);
                        if (pos->IsLegal()) {
                            int score = 0;
                            if (scid_TB_Probe (pos, &score) == OK) {
                                if (score == 0) { result = 0; }
                                else if (score > 0) { result = 1; } // STM wins
                                else {result = 2; } // STM loses
                            } else {
                                tbFailure("KPPKR", pos);
                            }
                        }
                    }
                    // Add the result to the table:
                    resultGrid->SetResult (result, wk, wr);
                    prevResult = result;
                }
            }
            writer->Add (resultGrid, bp1, bp2, bk, stm);
            resultGrid->UpdateStats ();
            blackSquares++;
        }
    }
    writer->Write (fp);
    resultGrid->PrintStats (stdout, "KPP-KR");
    delete resultGrid;
    delete pos;
    delete writer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// kppkp
//   Selected KPP-KP results.
//
void kppkp (FILE * fp)
{
    const uint bitsPerResult = 2;
    MTBWriter * writer = new MTBWriter ("KPPKP", bitsPerResult);
    Position * pos = new Position();
    ResultGrid * resultGrid = new ResultGrid(bitsPerResult);

    // The most common pawn file configuration (normalised so the black
    // pawn is on the queenside) are ab-a, ac-a, and ab-b.

    // PAWN_SQUARES: array of TRIPLET(wpSquare,wpSquare,bpSquare)
    uint PAWN_SQUARES[] = {
        // a+b pawns vs a-pawn
        TRIPLET(A6,B5,A7), TRIPLET(A5,B6,A7), TRIPLET(A5,B5,A7),
        TRIPLET(A4,B5,A7),
        TRIPLET(A5,B4,A6), TRIPLET(A4,B4,A6),

        // a+c pawns vs a-pawn

        // a+b pawns vs b-pawn

        TRIPLET(NS,NS,NS)
    };

    for (colorT stm = WHITE; stm <= BLACK; stm++) {
        uint * pawnSquares = PAWN_SQUARES;
        while (*pawnSquares != TRIPLET(NS,NS,NS)) {
            resultGrid->Clear();
            squareT wp1 = FIRST(*pawnSquares);
            squareT wp2 = SECOND(*pawnSquares);
            squareT bp = THIRD(*pawnSquares);
            uint prevResult = 0;  // Default value for a broken result

            for (squareT wk = A1; wk <= H8; wk++) {
                for (squareT bk = A1; bk <= H8; bk++) {
                    uint result = prevResult; // Broken: use previous result
                    if (uniqueSquares (wk, bk, wp1, wp2, bp)) {
                        pos->Clear();
                        pos->AddPiece(WK, wk);
                        pos->AddPiece(BK, bk);
                        pos->AddPiece(WP, wp1);
                        pos->AddPiece(WP, wp2);
                        pos->AddPiece(BP, bp);
                        pos->SetToMove(stm);
                        if (pos->IsLegal()) {
                            int score = 0;
                            if (scid_TB_Probe (pos, &score) == OK) {
                                if (score == 0) { result = 0; }
                                else if (score > 0) { result = 1; }
                                else {result = 2; }
                            } else {
                                tbFailure("KPPKP", pos);
                            }
                        }
                    }
                    // Add the result to the table:
                    resultGrid->SetResult (result, wk, bk);
                    prevResult = result;
                }
            }
            writer->Add (resultGrid, wp1, wp2, bp, stm);
            resultGrid->UpdateStats ();
            pawnSquares++;
        }
    }
    writer->Write (fp);
    resultGrid->PrintStats (stdout, "KPP-KP");
    delete resultGrid;
    delete pos;
    delete writer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main
int main (int argc, char ** argv)
{
    if (argc != 2) {
        fprintf (stderr, "Usage: mtbgen <TB-paths>\n");
        exit(1);
    }

    // Set a large tablebase cache size and initialize tablebases
    scid_TB_SetCacheSize (64 * 1024 * 1024);   // 64 MB cache
    uint npieces = scid_TB_Init (argv[1]);
    printf ("Tablebases with up to %u pieces were found\n", npieces);

    FILE * fp = fopen ("mtbdata.h", "wb");
    if (fp == NULL) {
        fprintf (stderr, "Error opening file\n");
        exit(1);
    }

    fprintf (fp, "%s\n", SLASHES);
    fprintf (fp, "//\n");
    fprintf (fp, "//  FILE:       mtbdata.h\n");
    fprintf (fp, "//              Memory tablebases\n");
    fprintf (fp, "//\n");
    fprintf (fp, "//  Part of:    Scid (Shane's Chess Information Database)\n");
    fprintf (fp, "//  Version:    3.5\n");
    fprintf (fp, "//\n");
    fprintf (fp, "//  Notice:     Copyright (c) 2003 Shane Hudson.  All rights reserved.\n");
    fprintf (fp, "//\n");
    fprintf (fp, "//  Author:     Shane Hudson (sgh@users.sourceforge.net)\n");
    fprintf (fp, "//\n");
    fprintf (fp, "%s\n", SLASHES);

    fprintf (fp, "\n\n");
    fprintf (fp, "// This file was automatically generated by the program mtbgen.cpp.\n");
    fprintf (fp, "// It contains selected compressed tablebase data used by the endgame\n");
    fprintf (fp, "// recognition code in the Scidlet chess engine.\n");
    fprintf (fp, "\n");
    fprintf (fp, "#ifndef SCID_MTBDATA_H\n");
    fprintf (fp, "#define SCID_MTBDATA_H\n");
    fprintf (fp, "\n\n");

    kqk (fp);
    krk (fp);
    kpk (fp);

    kqkq (fp);
    kqkp (fp);
    krkp (fp);
    kpkp (fp);

    // TODO: kqpkq (fp);
    krpkr (fp);
    // TODO: kppkr (fp);
    kppkp (fp);

    fprintf (fp, "\n");
    fprintf (fp, "%s\n", SLASHES);
    fprintf (fp, "\n\n");

    // Write the MTB initialization function:
    fprintf (fp, "static void initMTBs (void)\n{\n");
    fprintf (fp, "    initMTB_KQK();\n");
    fprintf (fp, "    initMTB_KRK();\n");
    fprintf (fp, "    initMTB_KPK();\n");
    fprintf (fp, "    initMTB_KQKQ();\n");
    fprintf (fp, "    initMTB_KQKP();\n");
    fprintf (fp, "    initMTB_KRKP();\n");
    fprintf (fp, "    initMTB_KPKP();\n");
    fprintf (fp, "    initMTB_KRPKR();\n");
    // TODO: fprintf (fp, "    initMTB_KPPKR();\n");
    fprintf (fp, "    initMTB_KPPKP();\n");
    fprintf (fp, "}\n\n\n");
    fprintf (fp, "#endif // SCID_MTBDATA_H\n");
    fprintf (fp, "\n");
    fprintf (fp, "%s\n", SLASHES);
    fprintf (fp, "//  EOF: mtbdata.h\n");
    fprintf (fp, "%s\n", SLASHES);

    fclose (fp);
    return 0;
}

//////////////////////////////////////////////////////////////////////
//  EOF: mtbgen.cpp
//////////////////////////////////////////////////////////////////////
