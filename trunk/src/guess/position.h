//////////////////////////////////////////////////////////////////////
//
//  FILE:       position.h
//              Position class
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 1999-2003 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


#ifndef SCID_POSITION_H
#define SCID_POSITION_H

#include "common.h"
#include "misc.h"
#include "movelist.h"
#include "sqlist.h"
#include "sqset.h"
#include "tokens.h"

#include <QtCore>

namespace Guess
{

//////////////////////////////////////////////////////////////////////
//  Position:  Constants

const unsigned char  WQ_CASTLE = 1,    WK_CASTLE = 2,
                     BQ_CASTLE = 4,    BK_CASTLE = 8;

// SANFlag: since checking if a move is check (to add the "+" to its
//      SAN string) takes time, and checking for mate takes even
//      longer, we specify whether we want this done with a flag.
typedef unsigned char      sanFlagT;
const sanFlagT    SAN_NO_CHECKTEST   = 0,
                  SAN_CHECKTEST      = 1,
                  SAN_MATETEST       = 2;


// Flags that Position::PrintFEN() recognises:
//
const unsigned int
FEN_COMPACT = 0,
FEN_BOARD = 1,
FEN_CASTLING_EP = 2,
FEN_ALL_FIELDS = 3;


// Flags that Position::GenerateMoves() recognises:
//
typedef unsigned int genMovesT;
const genMovesT
GEN_CAPTURES = 1,
GEN_NON_CAPS = 2,
GEN_ALL_MOVES = (GEN_CAPTURES | GEN_NON_CAPS);


// SANList: list of legal move strings in SAN.
//
struct sanListT
{
    bool        current;
    unsigned short      num;
    sanStringT  list [MAX_LEGAL_MOVES];
};


///////////////////////////////////////////////////////////////////////////
//  Position:  Class definition

class Position
{

private:
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //  Position:  Data structures

    pieceT          Board[66];      // the actual board + a color square
    // and a NULL square.
    unsigned int            Count[2];       // count of pieces & pawns each
    unsigned char            Material[16];   // count of each type of piece
    unsigned char            ListPos[64];    // ListPos stores the position in
    // List[][] for the piece on
    // square x.
    squareT         List[2][16];    // list of piece squares for each side
    unsigned char            NumOnRank[16][8];
    unsigned char            NumOnFyle[16][8];
    unsigned char            NumOnLeftDiag[16][16];  // Num Queens/Bishops
    unsigned char            NumOnRightDiag[16][16];
    unsigned char            NumOnSquareColor[16][2];

    directionT      Pinned[16];     // For each List[ToMove][x], stores
    // whether piece is pinned to its
    // own king and dir from king.

    squareT         EPTarget;       // square pawns can EP capture to
    colorT          ToMove;
    unsigned short          HalfMoveClock;  // Count of halfmoves since last capture
    // or pawn move.
    unsigned short          PlyCounter;
    unsigned char            Castling;       // castling flags
    bool            StrictCastling; // If false, allow castling after moving
    // the King or Rook.

    unsigned int            Hash;           // Hash value.
    unsigned int            PawnHash;       // Pawn structure hash value.

//    unsigned int            NumChecks;      // Number of checks.
//    SquareList      CheckSquares;   // Stores list of pieces checking the king.

    MoveList      * LegalMoves;     // list of legal moves
    sanListT      * SANStrings;     // SAN list of legal move strs


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //  Position:  Private Functions

    inline void AddHash(pieceT p, squareT sq);
    inline void UnHash(pieceT p, squareT sq);

    inline void AddToBoard(pieceT p, squareT sq);
    inline void RemoveFromBoard(pieceT p, squareT sq);

    void  CalcPinsDir(directionT dir, pieceT attacker);

    void  GenSliderMoves(MoveList * mlist, colorT c, squareT sq,
                         directionT dir, SquareSet * sqset,
                         bool capturesOnly);
    void  GenKnightMoves(MoveList * mlist, colorT c, squareT sq,
                         SquareSet * sqset, bool capturesOnly);

    void  AddLegalMove(MoveList * mlist, squareT from, squareT to, pieceT promo);
    void  GenCastling(MoveList * mlist);
    void  GenKingMoves(MoveList * mlist, genMovesT genType, bool castling);
    void  AddPromotions(MoveList * mlist, squareT from, squareT dest);
    bool  IsValidEnPassant(squareT from, squareT to);
    void  GenPawnMoves(MoveList * mlist, squareT from, directionT dir,
                       SquareSet * sqset, genMovesT genType);
    errorT      AssertPos();    //  Checks for errors in board etc.

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //  Position:  Public Functions
public:

    Position()
    {
        Init();
    }
    ~Position()
    {
        if(LegalMoves != NULL)
        {
            delete LegalMoves;
        }
        if(SANStrings != NULL)
        {
            delete SANStrings;
        }
    }

    void        Init();
    void        Clear();        // No pieces on board
    void        StdStart();     // Standard chess starting position
    bool        IsStdStart();
    errorT      AddPiece(pieceT p, squareT sq);

    // Set and Get attributes -- one-liners
    unsigned char        PieceCount(pieceT p)
    {
        return Material[p];
    }
    unsigned char *      GetMaterial()
    {
        return Material;
    }
    void        SetEPTarget(squareT s)
    {
        EPTarget = s;
    }
    squareT     GetEPTarget()
    {
        return EPTarget;
    }
    bool        GetEPFlag()
    {
        return (EPTarget != NS);
    }
    void        SetToMove(colorT c)
    {
        ToMove = c;
    }
    colorT      GetToMove()
    {
        return ToMove;
    }
    void        SetPlyCounter(unsigned short x)
    {
        PlyCounter = x;
    }
    unsigned short      GetPlyCounter()
    {
        return PlyCounter;
    }
    unsigned short      GetFullMoveCount()
    {
        return PlyCounter / 2 + 1;
    }
    sanListT *  GetSANStrings()
    {
        return SANStrings;
    }
    MoveList *  GetLegalMoves()
    {
        return LegalMoves;
    }

    // Methods to get the Board or piece lists -- used in game.cpp to
    // decode moves:
    squareT *   GetList(colorT c)
    {
        return List[c];
    }
    unsigned int        GetCount(colorT c)
    {
        return Count[c];
    }
    unsigned int        TotalMaterial()
    {
        return Count[WHITE] + Count[BLACK];
    }
    unsigned int        NumNonPawns(colorT c)
    {
        return Count[c] - Material[piece_Make(c, PAWN)];
    }
    bool        InPawnEnding()
    {
        return (NumNonPawns(WHITE) == 1  &&  NumNonPawns(BLACK) == 1);
    }
    unsigned int        MaterialValue(colorT c);
    inline unsigned int FyleCount(pieceT p, fyleT f)
    {
        return NumOnFyle[p][f];
    }
    inline unsigned int RankCount(pieceT p, rankT r)
    {
        return NumOnRank[p][r];
    }
    inline unsigned int LeftDiagCount(pieceT p, leftDiagT diag)
    {
        return NumOnLeftDiag[p][diag];
    }
    inline unsigned int RightDiagCount(pieceT p, rightDiagT diag)
    {
        return NumOnRightDiag[p][diag];
    }
    inline unsigned int SquareColorCount(pieceT p, colorT sqColor)
    {
        return NumOnSquareColor[p][sqColor];
    }
    unsigned int        GetSquares(pieceT p, SquareList * sqlist);

    pieceT *    GetBoard()
    {
        Board[COLOR_SQUARE] = COLOR_CHAR[ToMove];
        return Board;
    }

    // Other one-line methods
    squareT     GetKingSquare(colorT c)
    {
        return List[c][0];
    }
    squareT     GetKingSquare()
    {
        return List[ToMove][0];
    }
    squareT     GetEnemyKingSquare()
    {
        return List[1 - ToMove][0];
    }

    // Castling flags
    inline void SetCastling(colorT c, castleDirT dir, bool flag);
    bool        GetCastling(colorT c, castleDirT dir);
    inline bool CastlingPossible()
    {
        return (Castling ? true : false);
    }
    unsigned char        GetCastlingFlags()
    {
        return Castling;
    }
    void        SetCastlingFlags(unsigned char b)
    {
        Castling = b;
    }

    void        SetStrictCastling(bool b)
    {
        StrictCastling = b;
    }
    bool        GetStrictCastling(void)
    {
        return StrictCastling;
    }

    // Allocating memory  -- maybe these should be private??
    void        AllocLegalMoves();
    void        AllocSANStrings();

    // Clearing data structures
    void        ClearLegalMoves();
    void        ClearSANStrings();

    // Hashing
    inline unsigned int HashValue(void)
    {
        return Hash;
    }
    inline unsigned int PawnHashValue(void)
    {
        return PawnHash;
    }
    unsigned int GetHPSig();

    // Move generation and execution
    void        CalcPins();
    void        GenPieceMoves(MoveList * mlist, squareT sq,
                              SquareSet * sqset, bool capturesOnly);

    // Generate all legal moves:
    void  GenerateMoves(MoveList * mlist, pieceT mask, genMovesT genType, bool maybeInCheck);
    void  GenerateMoves()
    {
        GenerateMoves(NULL, EMPTY, GEN_ALL_MOVES, true);
    }
    void  GenerateMoves(MoveList * mlist)
    {
        GenerateMoves(mlist, EMPTY, GEN_ALL_MOVES, true);
    }
    void  GenerateMoves(MoveList * mlist, pieceT mask)
    {
        GenerateMoves(mlist, mask, GEN_ALL_MOVES, true);
    }
    void  GenerateMoves(MoveList * mlist, genMovesT genType)
    {
        GenerateMoves(mlist, EMPTY, genType, true);
    }
    void  GenerateCaptures(MoveList * mlist)
    {
        GenerateMoves(mlist, EMPTY, GEN_CAPTURES, true);
    }
    bool  IsLegalMove(simpleMoveT * sm);

    void        GenCheckEvasions(MoveList * mlist, pieceT mask, genMovesT genType, SquareList * checkSquares);
    void        MatchLegalMove(MoveList * mlist, pieceT mask, squareT target);
    errorT      MatchPawnMove(MoveList * mlist, fyleT fromFyle, squareT to, pieceT promote);
    errorT      MatchKingMove(MoveList * mlist, squareT target);

    unsigned int        CalcAttacks(colorT toMove, squareT kingSq, SquareList * squares);
    unsigned int        CalcNumChecks()
    {
        return CalcAttacks(1 - ToMove, GetKingSquare(), NULL);
    }
    unsigned int        CalcNumChecks(squareT kingSq)
    {
        return CalcAttacks(1 - ToMove, kingSq, NULL);
    }
    unsigned int        CalcNumChecks(squareT kingSq, SquareList * checkSquares)
    {
        return CalcAttacks(1 - ToMove, kingSq, checkSquares);
    }

    unsigned int        Mobility(pieceT p, colorT color, squareT from);
    pieceT      SmallestDefender(colorT color, squareT target);
    bool        IsKingInCheck()
    {
        return (CalcNumChecks() > 0);
    }
    bool        IsKingInCheckDir(directionT dir);
    bool        IsKingInCheck(simpleMoveT * sm);
    bool        IsKingInMate();
    bool        IsLegal();

    bool        IsPromoMove(squareT from, squareT to);

    void        DoSimpleMove(simpleMoveT * sm);     // move execution ...
    void        UndoSimpleMove(simpleMoveT * sm);   // ... and taking back

    errorT      RelocatePiece(squareT fromSq, squareT toSq);

    void        MakeSANString(simpleMoveT * sm, char * s, sanFlagT flag);
    void        CalcSANStrings(sanFlagT flag);

    errorT      ReadCoordMove(simpleMoveT * m, const char * s, bool reverse);
    errorT      ReadMove(simpleMoveT * m, const char * s, tokenT t);
    errorT      ParseMove(simpleMoveT * sm, const char * s);
    errorT      ReadLine(const char * s);

    // Board I/O
    void        MakeLongStr(char * str);
    void        DumpBoard(FILE * fp);
    void        DumpLists(FILE * fp);
    errorT      ReadFromLongStr(const char * str);
    errorT      ReadFromCompactStr(const unsigned char * str);
    errorT      ReadFromFEN(const char * s);
    void        PrintCompactStr(char * cboard);
    void        PrintCompactStrFlipped(char * cboard);
    unsigned char        CompactStrFirstByte()
    {
        return (Board[0] << 4) | Board[1];
    }
    void        PrintFEN(char * str, unsigned int flags);
    void        DumpLatexBoard(QString *dstr, bool flip = false);
    void        DumpHtmlBoard(QString *dstr, unsigned int style, const char * dir, bool flip = false);

    // Copy, compare positions
    int         Compare(Position * p);
    void        CopyFrom(Position * src);

    // Set up a random position:
    errorT      Random(const char * material);
};



//////////////////////////////////////////////////////////////////////
//  Position:  Public Inline Functions


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::SetCastling():
//      Set a castling flag.
//
inline void
Position::SetCastling(colorT c, castleDirT dir, bool flag)
{
    register unsigned char b = (c == WHITE ? 1 : 4);
    if(dir == KSIDE)
    {
        b += b;
    }
    // Now b = 1 or 2 (white flags), or 4 or 8 (black flags)
    if(flag)
    {
        Castling |= b;
    }
    else
    {
        Castling &= (255 - b);
    }
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GetCastling():
//      Get a castling flag.
//
inline bool
Position::GetCastling(colorT c, castleDirT dir)
{
    register unsigned char b = (c == WHITE ? 1 : 4);
    if(dir == KSIDE)
    {
        b += b;
    }
    // Now b == 1 or 2 (white flags), or 4 or 8 (black flags)
    if(Castling & b)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::AllocLegalMoves():
//      Allocate the legal moves list.
//
inline void
Position::AllocLegalMoves()
{
    ASSERT(LegalMoves == NULL);
    LegalMoves = new MoveList();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::AllocSANStrings():
//      Allocate the SAN strings list.
//
inline void
Position::AllocSANStrings()
{
    ASSERT(SANStrings == NULL);
    SANStrings = new sanListT;
    SANStrings->current = false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::ClearLegalMoves():
//      Reset the legal moves list.
//
inline void
Position::ClearLegalMoves()
{
    if(LegalMoves == NULL)
    {
        AllocLegalMoves();
    }
    LegalMoves->Clear();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::ClearSANStrings():
//      Reset the SAN moves list.
//
inline void
Position::ClearSANStrings()
{
    if(SANStrings)
    {
        SANStrings->current = false;
    }
    else
    {
        AllocSANStrings();
    }
}

} // End namespace Guess

#endif  // SCID_POSITION_H

//////////////////////////////////////////////////////////////////////
//  EOF: position.h
//////////////////////////////////////////////////////////////////////

