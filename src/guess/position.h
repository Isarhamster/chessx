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
#include "movelist.h"
#include "sqlist.h"
#include "sqset.h"

#include <QtCore>

namespace Guess
{

static const int Infinity    = 32000;
static const int KingValue   = 10000;
static const int QueenValue  =   900;
static const int RookValue   =   500;
static const int BishopValue =   300;
static const int KnightValue =   300;
static const int PawnValue   =   100;

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
    bool            Chess960Castling;
    quint64 CastlingRooks;

    unsigned int            Hash;           // Hash value.
    unsigned int            PawnHash;       // Pawn structure hash value.

    MoveList      * LegalMoves;     // list of legal moves
 
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //  Position:  Private Functions

    inline void AddHash(pieceT p, squareT sq);
    inline void UnHash(pieceT p, squareT sq);

    inline void AddToBoard(pieceT p, squareT sq);
    inline void RemoveFromBoard(pieceT p, squareT sq);

    void  CalcPinsDir(directionT dir, pieceC attacker);

    void  GenSliderMoves(MoveList * mlist, colorT c, squareT sq,
                         directionT dir, SquareSet * sqset,
                         bool capturesOnly) const;
    void  GenKnightMoves(MoveList * mlist, colorT c, squareT sq,
                         SquareSet * sqset, bool capturesOnly) const;

    void  AddLegalMove(MoveList * mlist, squareT from, squareT to, pieceC promotion = C_EMPTY, bool castle = false) const;
    void  GenCastling(MoveList * mlist) const;
    void  GenCastling960(MoveList * mlist) const;
    void  GenKingMoves(MoveList * mlist, genMovesT genType, bool castling);
    void  AddPromotions(MoveList * mlist, squareT from, squareT dest) const;
    bool  IsValidEnPassant(squareT from, squareT to);
    void  GenPawnMoves(MoveList * mlist, squareT from, directionT dir,
                       SquareSet * sqset, genMovesT genType);
    errorT      AssertPos();    //  Checks for errors in board etc.

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //  Position:  Public Functions
    bool isFreeForCastling960(squareT from, squareT to, squareT rook_from, squareT rook_to, squareT enemyKingSq) const;
    pieceT pieceAt(squareT square) const;
    squareT CastlingRook(int index) const;
    squareT CastlingRook(squareT rook) const;
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
    }

    void        Init();
    void        Clear();        // No pieces on board
    void        StdStart();     // Standard chess starting position
    bool        IsStdStart();
    errorT      AddPiece(pieceT p, squareT sq);

    // Set and Get attributes -- one-liners
    unsigned char PieceCount(pieceT p) const
    {
        return Material[p];
    }
    const unsigned char* GetMaterial() const
    {
        return Material;
    }
    unsigned char* GetMaterial()
    {
        return Material;
    }
    void SetEPTarget(squareT s)
    {
        EPTarget = s;
    }
    squareT     GetEPTarget() const
    {
        return EPTarget;
    }
    bool        GetEPFlag() const
    {
        return (EPTarget != NULL_SQUARE);
    }
    void        SetToMove(colorT c)
    {
        ToMove = c;
    }
    colorT      GetToMove() const
    {
        return ToMove;
    }
    void        SetPlyCounter(unsigned short x)
    {
        PlyCounter = x;
    }
    unsigned short      GetPlyCounter() const
    {
        return PlyCounter;
    }
    unsigned short      GetFullMoveCount() const
    {
        return PlyCounter / 2 + 1;
    }
    MoveList *  GetLegalMoves() const
    {
        return LegalMoves;
    }
    inline int
    ScoreWhiteMaterial(void)  const
    {
        const unsigned char * pieceCount = GetMaterial();
        return  pieceCount[WQ] * QueenValue   +  pieceCount[WR] * RookValue
                +  pieceCount[WB] * BishopValue  +  pieceCount[WN] * KnightValue
                +  pieceCount[WP] * PawnValue;
    }

    inline int
    ScoreBlackMaterial(void)  const
    {
        const unsigned char * pieceCount = GetMaterial();
        return  pieceCount[BQ] * QueenValue   +  pieceCount[BR] * RookValue
                +  pieceCount[BB] * BishopValue  +  pieceCount[BN] * KnightValue
                +  pieceCount[BP] * PawnValue;
    }

    inline int
    ScoreMaterial(void) const
    {
        int score = ScoreWhiteMaterial() - ScoreBlackMaterial();
        return score;
    }

    // Methods to get the Board or piece lists -- used in game.cpp to
    // decode moves:
    squareT* GetList(colorT c)
    {
        return List[c];
    }
    unsigned int GetCount(colorT c)  const
    {
        return Count[c];
    }
    unsigned int        TotalMaterial()  const
    {
        return Count[WHITE] + Count[BLACK];
    }
    unsigned int        NumNonPawns(colorT c)  const
    {
        return Count[c] - Material[piece_Make(c, PAWN)];
    }
    bool        InPawnEnding() const
    {
        return (NumNonPawns(WHITE) == 1  &&  NumNonPawns(BLACK) == 1);
    }
    unsigned int        MaterialValue(colorT c);
    inline unsigned int FyleCount(pieceT p, fyleT f) const
    {
        return NumOnFyle[p][f];
    }
    inline unsigned int RankCount(pieceT p, rankT r) const
    {
        return NumOnRank[p][r];
    }
    inline unsigned int LeftDiagCount(pieceT p, leftDiagT diag) const
    {
        return NumOnLeftDiag[p][diag];
    }
    inline unsigned int RightDiagCount(pieceT p, rightDiagT diag) const
    {
        return NumOnRightDiag[p][diag];
    }
    inline unsigned int SquareColorCount(pieceT p, colorT sqColor) const
    {
        return NumOnSquareColor[p][sqColor];
    }
    unsigned int        GetSquares(pieceT p, SquareList * sqlist);

    pieceT* GetBoard()
    {
        Board[COLOR_SQUARE] = pieceT(COLOR_CHAR[ToMove]);
        return Board;
    }

    // Other one-line methods
    squareT     GetKingSquare(colorT c) const
    {
        return List[c][0];
    }
    squareT     GetKingSquare() const
    {
        return List[ToMove][0];
    }
    squareT     GetEnemyKingSquare() const
    {
        return List[1 - ToMove][0];
    }

    // Castling flags
    inline void SetCastling(colorT c, castleDirT dir, bool flag);
    bool        GetCastling(colorT c, castleDirT dir) const;
    inline bool CastlingPossible() const
    {
        return (Castling ? true : false);
    }
    unsigned char GetCastlingFlags() const
    {
        return Castling;
    }
    void        SetCastlingFlags(unsigned char b)
    {
        Castling = b;
    }

    // Allocating memory  -- maybe these should be private??
    void        AllocLegalMoves();
 
    // Clearing data structures
    void        ClearLegalMoves();

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
                              SquareSet * sqset, bool capturesOnly) const;

    // Generate all legal moves:
    void  GenerateMoves(MoveList * mlist, pieceC mask, genMovesT genType, bool maybeInCheck);
    void  GenerateMoves()
    {
        GenerateMoves(NULL, C_EMPTY, GEN_ALL_MOVES, true);
    }
    void  GenerateMoves(MoveList * mlist)
    {
        GenerateMoves(mlist, C_EMPTY, GEN_ALL_MOVES, true);
    }
    void  GenerateMoves(MoveList * mlist, pieceC mask)
    {
        GenerateMoves(mlist, mask, GEN_ALL_MOVES, true);
    }
    void  GenerateMoves(MoveList * mlist, genMovesT genType)
    {
        GenerateMoves(mlist, C_EMPTY, genType, true);
    }
    void  GenerateCaptures(MoveList * mlist)
    {
        GenerateMoves(mlist, C_EMPTY, GEN_CAPTURES, true);
    }
    bool  IsLegalMove(simpleMoveT * sm);

    void        GenCheckEvasions(MoveList * mlist, pieceC mask, genMovesT genType, SquareList * checkSquares);
    void        MatchLegalMove(MoveList * mlist, pieceC mask, squareT target);
    errorT      MatchPawnMove(MoveList * mlist, fyleT fromFyle, squareT to, pieceC promote);
    errorT      MatchKingMove(MoveList * mlist, squareT target);

    unsigned int CalcAttacks(colorT toMove, squareT kingSq, SquareList * squares = 0, bool calcDiscoveredAttacks=0) const;
    unsigned int CalcNumChecks() const
    {
        return CalcAttacks(1 - ToMove, GetKingSquare(), NULL);
    }
    unsigned int CalcNumChecks(squareT kingSq) const
    {
        return CalcAttacks(1 - ToMove, kingSq, NULL);
    }
    unsigned int CalcNumChecks(squareT kingSq, SquareList * checkSquares) const
    {
        return CalcAttacks(1 - ToMove, kingSq, checkSquares);
    }

    unsigned int        Mobility(pieceC p, colorT color, squareT from);
    pieceC SmallestDefender(colorT color, squareT target) const;
    bool        IsKingInCheck() const
    {
        return (CalcNumChecks() > 0);
    }
    bool        IsKingInCheckDir(directionT dir) const;
    bool        IsKingInCheck(simpleMoveT * sm) const;
    bool        IsKingInMate();
    bool        IsLegal() const;

    bool        IsPromoMove(squareT from, squareT to);

    void        DoSimpleMove(simpleMoveT * sm);     // move execution ...
    void        UndoSimpleMove(simpleMoveT * sm);   // ... and taking back

    errorT      RelocatePiece(squareT fromSq, squareT toSq);

    void        MakeSANString(simpleMoveT * sm, char * s, sanFlagT flag);

    // Board I/O
    void        DumpBoard(FILE * fp);
    void        DumpLists(FILE * fp);
    errorT      ReadFromFEN(const char * s);

    // Copy, compare positions
    void        CopyFrom(Position * src);

    bool getChess960Castling() const;
    void setChess960Castling(bool value, quint64 castlingRooks);
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
    unsigned char b = (c == WHITE ? 1 : 4);
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
Position::GetCastling(colorT c, castleDirT dir) const
{
    unsigned char b = (c == WHITE ? 1 : 4);
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
    LegalMoves->clear();
}


} // End namespace Guess

#endif  // SCID_POSITION_H

//////////////////////////////////////////////////////////////////////
//  EOF: position.h
//////////////////////////////////////////////////////////////////////

