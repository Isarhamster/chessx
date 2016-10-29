/***************************************************************************
 *   (C) 2003 Sune Fischer                                                 *
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "movelist.h"
#include "piece.h"
#include "square.h"

#ifndef __BITBOARD_H__
#define __BITBOARD_H__

enum BoardStatus
{
    Valid, NoWhiteKing, NoBlackKing, DoubleCheck,
    OppositeCheck, TooManyBlackPawns, TooManyWhitePawns,
    PawnsOn18, TooManyKings, TooManyWhite, TooManyBlack,
    BadCastlingRights, InvalidEnPassant, MultiCheck
};

typedef uchar CastlingRights;
enum
{
    NoRights = 0,
    WhiteKingside = 1, BlackKingside = 2,
    WhiteQueenside = 4, BlackQueenside = 8,
    WhiteBothSides = 5, BlackBothSides = 10,
    AllRights = 15
};

/** @ingroup Core
 * Keep track of the pieces on a chessboard, provide a Move() factory.
 */
class BitBoard
{
public:
    BitBoard();

    // Play moves on board
    //
    /** Play given move, updating board state appropriately */
    bool doMove(const Move&);
    /** Return board to state prior to given move */
    void undoMove(const Move&);

    // Setup board
    //
    /** Remove all pieces and state from board */
    void clear();
    /** Set move number in game */
    void setMoveNumber(unsigned int moveNumber);
    /** Set initial chess game position on the board */
    void setStandardPosition();
    /** Set the given piece on the board at the given square */
    void setAt(const Square s, const Piece p);
    /** Remove any piece sitting on given square */
    void removeAt(const Square s);
    /** Set side to move as that of given color */
    void setToMove(const Color& c);
    /** Swap the side to move */
    void swapToMove();
    /** Parse given FEN, return true if loaded properly otherwise false */
    bool fromFen(const QString& fen);
    /** Set En Passant Square */
    void setEnPassantSquare(const Square s);
    /** Set En Passant in a file */
    void setEnPassantFile(int f);
    /** From a FICS representation of a board */
    bool from64Char(const QString &qcharboard);
    /** Remove En Passant privilege */
    void clearEnPassantSquare();

    // Move factories
    //
    /** parse SAN or LAN representation of move, and return proper Move() object */
    Move parseMove(const QString& algebraic) const;
    /** Return a proper Move() object given only a from-to move specification */
    Move prepareMove(const Square& from, const Square& to) const;

    // Return a nullMove -- King to the same square
    Move nullMove() const;

    // Query
    //
    /** Is piece sitting on given square moveable ? */
    bool isMovable(const Square s) const;
    /** @return piece sitting at given square on the board */
    Piece pieceAt(Square s) const;
    /* Get the color of a Piece at square @s */
    Color colorAt(Square s) const;
    /** @return number of ply since a pawn move or capture */
    unsigned int halfMoveClock() const;
    /** Set number of ply since a pawn move or capture */
    void setHalfMoveClock(unsigned int i);
    /** @return the current move number in the game */
    unsigned int moveNumber() const;
    /** @return color of side next to move */
    Color toMove() const;
    /** @return true if its possible for this position to follow target position */
    bool canBeReachedFrom(const BitBoard& target) const;
    /** @return true if position is same, but don't consider Move # in determination */
    bool positionIsSame(const BitBoard& target) const;
    /** @return true if neither side can win the game */
    bool insufficientMaterial() const;

    // Query other formats
    //
    /** Return a FEN string based on current board position */
    QString toFen() const;
    /** Return a FEN string in human readable format based on current board position */
    QString toHumanFen() const;
    /** Return a SAN string representation of given move */
    QString moveToSan(const Move& move, bool translate = false) const;
    /** @return a SAN string representing a given move with move number. */
    QString moveToFullSan(const Move& move) const;

    // Validation
    //
    /** Check current position and return "Valid" or problem */
    BoardStatus validate() const;
    /** Return true if given FEN can be parsed */
    bool isValidFen(const QString& fen) const;

    /** Set castling rights. */
    void setCastlingRights(CastlingRights cr);
    /** Return the internal castling rights data (used by hash function) */
    CastlingRights castlingRights() const;
    /** Return square where En passant capture may occur, or "NoEPSquare" */
    Square enPassantSquare() const;

    bool canTakeEnPassant() const;
    /** Return true if the side to move is in checkmate */
    bool isCheckmate() const;
    /** Return true if the side to move is stalemated */
    bool isStalemate() const;

    /** Return 1 if Chess960 is selected, 0 otherwise */
    bool chess960() const;
    /** Set to 1 if Chess960 is selected, 0 otherwise */
    void setChess960(bool chess960);
    /** Setup board according to position number */
    void fromChess960pos(int value);
    /** Get the position number from the current position */
    int chess960Pos() const;
    quint64 castlingRooks() const;
    static quint64 standardCastlingRooks();

    void setCastlingRooks(char file000=0, char file00=0);
    int CastlingRookIndex(Square rook) const;
    bool HasRookOnFileForCastling(unsigned char file, bool castle000) const;

    bool hasAmbiguousCastlingRooks(char file000 = 0, char file00 = 0) const;
    /** Test to see if given color has the right to castle on kingside */
    bool canCastleShort(const unsigned int color) const;
    /** Test to see if given color has the right to castle on queenside */
    bool canCastleLong(const unsigned int color) const;
    /** Get the castling rook to the given index into m_castlingRook */
    Square CastlingRook(int index) const;
    /** Return the square of the king if in check, InvalidSquare otherwise */
    Square kingInCheck() const;
protected:
    unsigned int countSetBits(quint64 n) const;
private:
    /** Test if a king is on a certain row to test castling rights */
    bool isKingOnRow(Piece p, Square start, Square stop) const;
    /** Return true if side to move is in check */
    bool isCheck() const;

    /** Test to see if given color has any castling rights remaining */
    bool canCastle(const unsigned int color) const;

    /** Return true if making move would put oneself into check */
    bool isIntoCheck(const Move& move) const;
    /** Return true if the given square is attacked by the given color */
    bool isAttackedBy(const unsigned int color, Square square) const;
    bool isAttackedBy(const unsigned int color, Square start, Square stop) const;

    /** Return all squares attacked by a knight on given square */
    quint64 knightAttacksFrom(const Square s) const;
    /** Return all squares attacked by a bishop on given square */
    quint64 bishopAttacksFrom(const Square s) const;
    /** Return all squares attacked by a rook on given square */
    quint64 rookAttacksFrom(const Square s) const;
    /** Return all squares attacked by a queen on given square */
    quint64 queenAttacksFrom(const Square s) const;
    /** Return all squares attacked by a king on given square */
    quint64 kingAttacksFrom(const Square s) const;
    /** Return all possible pawn moves from given square */
    quint64 pawnMovesFrom(const Square s) const;

    /** Remove impossible moves from given bitboard to aid disambiguation */
    void removeIllegal(const Move& move, quint64& b) const;
    /** Update move with castling details, return false if no castle is possible */
    bool prepareCastle(Move& move) const;
    /** Update move with castling details for Chess960, return false if no castle is possible */
    bool prepareCastle960(Move &move) const;
    /** Test that nothing is inbetween the castling pieces */
    bool isFreeForCastling960(Square from, Square to, Square rook_from, Square rook_to) const;
    /** Generate all possible moves in a given position */
    MoveList generateMoves() const;

    /** Grant castling rights on the kingside to the given color */
    void setCastleShort(unsigned int color);
    /** Grant castling rights on the queenside to the given color */
    void setCastleLong(unsigned int color);
    /** Revoke all castling rights from the given color */
    void destroyCastle(unsigned int color);
    /** Revoke castling rights from the given color */
    void destroyCastleInDirection(unsigned int color, Square s);
    /** Update the epSquare value based on a new epFile value */
    void epFile2Square();

    /** Setup board according to FEN string */
    bool fromGoodFen(const QString& fen, bool chess960=false);
    /** Get the rook with index from castling rook storage */
    bool HasRookForCastling(int index) const;
    Square CastlingKingTarget(int rookIndex) const;
    Square CastlingRookTarget(int rookIndex) const;
    void fixCastlingRooks(bool, char file000=0, char file00=0);

    // Actual Bit-board data
    quint64 m_pawns, m_knights, m_bishops, m_rooks, m_castlingRooks, m_queens, m_kings;
    quint64 m_occupied_co[2];     // Square mask of those occupied by each color
    quint64 m_occupied;           // Square is empty or holds a piece
    quint64 m_occupied_l90;       // rotated counter clockwise 90 deg
    quint64 m_occupied_l45;       // an odd transformation, to straighten out diagonals
    quint64 m_occupied_r45;       // the opposite odd transformation, just as messy

    // Extra state data
    unsigned char m_piece[64];             // type of piece on this square
    unsigned char m_stm;                   // side to move
    Square        m_ksq[2];                // square of the m_kings
    unsigned char m_epFile;                // file of a possible ep capture
    unsigned char m_epSquare;              // This is requested by hash routine enough that we keep it pre calculated
    unsigned char m_castle;                // flags for castle legality  (these can be merged)
    unsigned short m_halfMoves;            // Number of moves since last pawn move or capture
    unsigned int m_moveNumber;             // Move number in game (incremented after each black move)
    unsigned char m_pawnCount[2];          // Number of pawns for each side
    unsigned char m_pieceCount[2];         // Number of pieces INCLUDING pawns for each side
    unsigned char m_chess960;              // 0 = standard, 1 = Chess960
};

enum Char64Position
{
    C64_ROW8,
    C64_ROW7,
    C64_ROW6,
    C64_ROW5,
    C64_ROW4,
    C64_ROW3,
    C64_ROW2,
    C64_ROW1,
    C64_COLOR_TO_MOVE,
    C64_COL_DOUBLE_PAWN_PUSH,
    C64_CASTLE_W_00,
    C64_CASTLE_W_000,
    C64_CASTLE_B_00,
    C64_CASTLE_B_000,
    C64_PLY_50_MOVE,
    C64_NUM_GAME,
    C64_NAME_WHITE,
    C64_NAME_BLACK,
    C64_GAME_RELATION,
    C64_INITIAL_TIME_S,
    C64_INCREMENT_S,
    C64_MATERIAL_WHITE,
    C64_MATERIAL_BLACK,
    C64_REMAINDER_TIME_WHITE,
    C64_REMAINDER_TIME_BLACK,
    C64_NEXT_MOVE_NUMBER,
    C64_LAST_MOVE,
    C64_ELAPSED_TIME_LAST_MOVE,
    C64_PP_LAST_MOVE,
    C64_FLIP_BOARD,
    C64_CLOCK_RUNNING,
    C64_LAG
};

enum Char64Relation
{
    C64_REL_ISOLATED = -3,           // isolated position, such as for "ref 3" or the "sposition" command
    C64_REL_OBSERVING_EXAMINE = -2,  // I am observing game being examined
    C64_REL_PLAY_OPPONENT_MOVE = -1, // I am playing, it is my opponent's move
    C64_REL_OBSERVING_PLAY = 0,      // I am observing a game being played
    C64_REL_PLAY_MY_MOVE = 1,        // I am playing and it is my move
    C64_REL_EXAMINE = 2              // I am the examiner of this game
};

extern quint64 bb_PawnAttacks[2][64];
extern quint64 bb_KnightAttacks[64];
extern quint64 bb_R45Attacks[64][64];
extern quint64 bb_L45Attacks[64][64];
extern quint64 bb_KingAttacks[64];
extern quint64 bb_RankAttacks[64][64];
extern quint64 bb_FileAttacks[64][64];

const unsigned int bb_ShiftR45[64] =
{
    1, 58, 51, 44, 37, 30, 23, 16,
    9, 1, 58, 51, 44, 37, 30, 23,
    17, 9, 1, 58, 51, 44, 37, 30,
    25, 17, 9, 1, 58, 51, 44, 37,
    33, 25, 17, 9, 1, 58, 51, 44,
    41, 33, 25, 17, 9, 1, 58, 51,
    49, 41, 33, 25, 17, 9, 1, 58,
    57, 49, 41, 33, 25, 17, 9, 1
};

const unsigned int bb_ShiftL45[64] =
{
    9, 17, 25, 33, 41, 49, 57, 1,
    17, 25, 33, 41, 49, 57, 1, 10,
    25, 33, 41, 49, 57, 1, 10, 19,
    33, 41, 49, 57, 1, 10, 19, 28,
    41, 49, 57, 1, 10, 19, 28, 37,
    49, 57, 1, 10, 19, 28, 37, 46,
    57, 1, 10, 19, 28, 37, 46, 55,
    1, 10, 19, 28, 37, 46, 55, 64
};

inline bool BitBoard::isAttackedBy(const unsigned int color, Square square) const
{
    if(bb_PawnAttacks[color ^ 1][square] & (m_pawns | m_bishops) & m_occupied_co[color])
    {
        return 1;
    }
    if(knightAttacksFrom(square) & m_knights & m_occupied_co[color])
    {
        return 1;
    }
    if(bishopAttacksFrom(square) & (m_bishops | m_queens) & m_occupied_co[color])
    {
        return 1;
    }
    if(rookAttacksFrom(square) & (m_rooks | m_queens) & m_occupied_co[color])
    {
        return 1;
    }
    if(kingAttacksFrom(square) & (m_kings | m_queens) & m_occupied_co[color])
    {
        return 1;
    }
    return 0;
};

inline bool BitBoard::isAttackedBy(const unsigned int color, Square start, Square stop) const
{
    Square square = start;

    while(square!=stop)
    {
        bool underAttack = isAttackedBy(color, square);
        if (underAttack) return true;
        if (square!=stop) square += (start<=stop) ? 1:-1;
    }

    return isAttackedBy(color, stop);
};

inline void BitBoard::setCastleShort(unsigned int color)
{
    m_castle |= 1 << color;
}

inline void BitBoard::setCastleLong(unsigned int color)
{
    m_castle |= 4 << color;
}

inline void BitBoard::destroyCastle(unsigned int color)
{
    m_castle &= ~(5 << color);
}

inline void BitBoard::destroyCastleInDirection(unsigned int color, Square s)
{
    for (int i = 0; i<4; ++i)
    {
        if (CastlingRook(i) == s)
        {
            switch(i)
            {
            case 0: if (color == 0) m_castle &= (unsigned char) ~4; break;
            case 1: if (color == 0) m_castle &= (unsigned char) ~1; break;
            case 2: if (color == 1) m_castle &= (unsigned char) ~8; break;
            case 3: if (color == 1) m_castle &= (unsigned char) ~2; break;
            }
            break;
        }
    }
}

inline quint64 BitBoard::knightAttacksFrom(const Square s) const
{
    return bb_KnightAttacks[s];
}

inline quint64 BitBoard::bishopAttacksFrom(const Square s) const
{
    return bb_R45Attacks[s][(m_occupied_r45 >> bb_ShiftR45[s]) & 63] |
           bb_L45Attacks[s][(m_occupied_l45 >> bb_ShiftL45[s]) & 63];
}

inline quint64 BitBoard::rookAttacksFrom(const Square s) const
{
    return bb_RankAttacks[s][(m_occupied >> ((s & ~7) + 1)) & 63] |
           bb_FileAttacks[s][(m_occupied_l90 >> (((s & 7) << 3) + 1)) & 63];
}

inline quint64 BitBoard::queenAttacksFrom(const Square s) const
{
    return rookAttacksFrom(s) | bishopAttacksFrom(s);
}

inline quint64 BitBoard::kingAttacksFrom(const Square s)  const
{
    return bb_KingAttacks[s];
}

inline void BitBoard::epFile2Square()
{
    if(m_epFile)
    {
        m_epSquare = m_epFile + (m_stm == White ? a6 : a3) - 1;
    }
    else
    {
        m_epSquare = NoEPSquare;
    }
}

inline bool BitBoard::canCastle(const unsigned int color) const
{
    return m_castle & (5 << color);
}

inline bool BitBoard::canCastleShort(const unsigned int color) const
{
    return m_castle & (1 << color);
}

inline bool BitBoard::canCastleLong(const unsigned int color)  const
{
    return m_castle & (4 << color);
}

inline bool BitBoard::isCheck() const
{
    return isAttackedBy(m_stm ^ 1, m_ksq[m_stm]);
}

inline Square BitBoard::kingInCheck() const
{
    return isCheck() ? m_ksq[m_stm] : (Square) InvalidSquare;
}

inline unsigned int BitBoard::halfMoveClock() const
{
    return m_halfMoves;
}

inline void BitBoard::setHalfMoveClock(unsigned int i)
{
    m_halfMoves = i;
}

inline unsigned int BitBoard::moveNumber() const
{
    return m_moveNumber;
}

inline Color BitBoard::toMove() const
{
    return Color(m_stm);
}

inline Square BitBoard::enPassantSquare() const
{
    return Square(m_epSquare);
}

inline CastlingRights BitBoard::castlingRights() const
{
    return m_castle;
}

inline void BitBoard::setCastlingRights(CastlingRights cr)
{
    m_castle = cr;
}

/** Set the side to move to the given color */
inline void BitBoard::setToMove(const Color& c)
{
    m_stm = c;
}

inline void BitBoard::swapToMove()
{
    m_stm ^= 1;
}

inline void BitBoard::setMoveNumber(unsigned int moveNumber)
{
    m_moveNumber = moveNumber;
}

inline bool BitBoard::positionIsSame(const BitBoard& target) const
{
    if(m_occupied_co[White] != target.m_occupied_co[White] ||
            m_occupied_co[Black] != target.m_occupied_co[Black] ||
            m_pawns != target.m_pawns ||
            m_knights != target.m_knights ||
            m_bishops != target.m_bishops ||
            m_rooks != target.m_rooks ||
            m_queens != target.m_queens ||
            m_kings != target.m_kings ||
            m_stm != target.m_stm)
    {
        return false;
    }
    return true;
}

#endif // __BITBOARD_H__
