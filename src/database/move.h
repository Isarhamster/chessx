/***************************************************************************
 *   (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>           *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __MOVE_H__
#define __MOVE_H__

#include "common.h"

class BitBoard;

/** @ingroup Core
   Moves are dependent on current position, (remembers piece, check, capture etc)
   and don't make much sense when considered without a Board.
   However, you can create a move with only a source and destination square,
   Such moves are considered "illegal", but can be convenient when dealing with move entry.

	@todo Clean or merge setPromotionPiece() and setPromoted() functions. When setting move from scratch
	(e. g. in @ref Shredder class), setPromotionPiece() fails probably because isPromotion() returns
	@p false .
*/

class Move
{
public:

    /** Default constructor, creates an empty (illegal) move */
    Move();

    /** Move entry constructor, untested (illegal) move with only from, and to squares set */
    Move(const Square from, const Square to);

    /** Set promotion piece.  Default promotion is to Queen, use this to change piece afterward */
    void setPromotionPiece(PieceType type);
    /** Set type of piece (Queen, Rook, Bishop, Knight, Pawn) pawn promoted to */
    void setPromoted(const uchar p);

    // Query
    /** return Square piece sits on after move */
    Square to() const;
    /** return Square piece sat on before move */
    Square from() const;
    /** return Square where rook is placed after castling */
    Square castlingRookTo() const;
    /** return Square when en-passant is captured. Undefined if there is no en-passant. */
    Square enPassantSquare() const;
    /** return Square where rook was placed before castling */
    Square castlingRookFrom() const;
    /** Convert to algebraic notation (e2e4, g8f6 etc.) */
    QString toAlgebraic() const;

    /** Get the piece type moving -- note, returns Pawn, QUEEN, etc.. not colorized */
    Piece pieceMoved() const;
    /** Piece captured from the opponent by this move */
    Piece capturedPiece() const;
    /** If move is promotion, get promotion piece. Result is undefined if there is no promotion */
    Piece promotedPiece() const;

    /** Check whether a given move is a null move ( an illegal move by the king to its own square ) often used as a placeholder in ebooks */
    bool isNullMove() const;

    /** Check whether move is special (promotion, castling, en passant */
    bool isSpecial() const;
    /** Check whether move is a promotion */
    bool isPromotion() const;
    /** Check whether move is a castling */
    bool isCastling() const;
    /** Check whether the move is a pawn double advance */
    bool isDoubleAdvance() const;
    /** Check whether move is an en passant */
    bool isEnPassant() const;
    /** Check if move is completely legal in the context it was created */
    bool isLegal() const;

    Color color() const;

    /** Return true if this move was made by given color */
    bool operator==(const Color& color) const;
    /** Return true if this move was NOT made by given color */
    bool operator!=(const Color& color) const;
    /** Return true if this move was made by given piece */
    bool operator==(const Piece& p) const;
    /** Return true if this move was NOT made by given piece */
    bool operator!=(const Piece& p) const;


    /** Moves are considered the same, only if they match exactly */
    friend bool operator==(const Move& m1, const Move& m2);
    /** Required for keeping moves in some map-like structures */
    friend bool operator<(const Move& m1, const Move& m2);

    friend class BitBoard;
private:
    static const quint64 CASTLE = 9;
    static const quint64 TWOFORWARD = 22;
    static const quint64 PROMOTE = 38;
    static const quint64 ENPASSANT = 14;

    static const quint64 CASTLINGBIT =  1 << 15;
    static const quint64 TWOFORWARDBIT = 1 << 16;
    static const quint64 PROMOTEBIT = 1 << 17;
    static const quint64 ENPASSANTBIT = 1 << 21;
    static const quint64 LEGALITYBIT =  1 << 31;
    static const quint64 SPECIALBITS = CASTLINGBIT | TWOFORWARDBIT | PROMOTEBIT | ENPASSANTBIT;
    static const quint64 PTCLEAR = ~(7 << 12);
    static const quint64 CAPCLEAR = ~(7 << 18);
    static const quint64 PROCLEAR = ~((7 << 22) | PROMOTEBIT);
    static const quint64 BLACKTM = 1 << 26;

    /** Set Pawn move of one forward */
    void genOneForward(unsigned int from, unsigned int to);
    /** Set two-squares forward move of Pawn */
    void genTwoForward(unsigned int from, unsigned int to);
    /** Set pawn promotion move to given Piecetype */
    void genPromote(unsigned int from, unsigned int to, unsigned int type);
    /** Set pawn capture and promotion, promote to piece type, capturing type */
    void genCapturePromote(unsigned int from, unsigned int to, unsigned int type, unsigned int captured);
    /** Set pawn en passant capture of opponent pawn */
    void genEnPassant(unsigned int from, unsigned int to);
    /** Set simple pawn move with capture of piece type */
    void genPawnMove(unsigned int from, unsigned int to, unsigned int captured);
    /** Set knight move, capturing piece type */
    void genKnightMove(unsigned int from, unsigned int to, unsigned int captured);
    /** Set bishop move, capturing piece type */
    void genBishopMove(unsigned int from, unsigned int to, unsigned int captured);
    /** Set rook move, capturing piece type */
    void genRookMove(unsigned int from, unsigned int to, unsigned int captured);
    /** Set queen move, capturing piece type */
    void genQueenMove(unsigned int from, unsigned int to, unsigned int captured);
    /** Set king move, capturing piece type */
    void genKingMove(unsigned int from, unsigned int to, unsigned int captured);
    /** Set castling short move for White with king and rook */
    void genWhiteOO();
    /** Set castling long move for White with king and rook */
    void genWhiteOOO();
    /** Set castling short move for Black with king and rook */
    void genBlackOO();
    /** Set castling long move for Black with king and rook */
    void genBlackOOO();
    /** Mark this move as validated and fully legal in position */
    void setLegalMove();
    /** Set source square for this move */
    void setFrom(Square from);
    /** Set destination square for this move */
    void setTo(Square to);
    /** Mark this move as being made by the Black side */
    void setBlack();
    /** Return piece type of promoted piece (or 0 if none) */
    unsigned int promoted() const;
    /** Set type of piece (Queen, Rook, Bishop, Knight, Pawn) making move */
    void setPieceType(const uchar p);
    /** Set type of piece (Queen, Rook, Bishop, Knight, Pawn) captured */
    void setCaptureType(const uchar p);
    /** Mark this move as an initial pawn move of 2 squares */
    void setTwoForward();
    /** Mark this move capturing a pawn en passant */
    void setEnPassant();
    /** Mark this move as giving check */
    void setCheck();
    /** Mark this move as giving checkmate */
    void setMate();

    /** Return pawn2forward, castle or piece type for doMove() and undoMove() */
    unsigned int action() const;
    /** Return captured piece or En passant for doMove() and undoMove() */
    unsigned int removal() const;
    /** Return piece type of captured piece (or 0 if none) */
    unsigned int capturedType() const;

    // The move definition 'm' bitfield layout:
    // 00000000 00000000 00000000 00111111 = from square     = bits 1-6
    // 00000000 00000000 00001111 11000000 = to square       = bits 7-12
    // 00000000 00000000 01110000 00000000 = piece type      = bits 13-15
    // 00000000 00000000 10000000 00000000 = castle	         = bit  16
    // 00000000 00000001 00000000 00000000 = pawn 2 forward  = bit  17
    // 00000000 00000010 00000000 00000000 = promotion       = bit  18
    // 00000000 00011100 00000000 00000000 = capture piece   = bits 19-21
    // 00000000 00100000 00000000 00000000 = en passant      = bit  22
    // 00000001 11000000 00000000 00000000 = promotion piece = bits 23-25
    // 00000010 00000000 00000000 00000000 = Extra data set  = bits 26  // NOT YET IMPLEMENTED
    // 00000100 00000000 00000000 00000000 = White=0,Black=1 = bit  27
    // 00001000 00000000 00000000 00000000 = SAN needs file  = bit  28  // NOT YET IMPLEMENTED
    // 00010000 00000000 00000000 00000000 = SAN needs rank  = bit  29  // NOT YET IMPLEMENTED
    // 00100000 00000000 00000000 00000000 = gives mate?     = bit  30  // NOT YET IMPLEMENTED
    // 01000000 00000000 00000000 00000000 = gives check?    = bit  31  // NOT YET IMPLEMENTED
    // 10000000 00000000 00000000 00000000 = legality status = bit  32
    unsigned int m;
    // The undo definition 'u' bitfield layout:
    // 00000000 11111111 = half move clock  = bits 1-8
    // 00001111 00000000 = castling rights  = bits 8-12
    // 11110000 00000000 = previous ep file = bits 13-16
    unsigned short u;
};

// return true if a null move
// that is moving a king to its same square
// while it is not legal it is often used
// to annotate ideas
inline bool Move::isNullMove() const
{

    if(from() == to())
    {
        Piece p = pieceMoved();
        PieceType pt = pieceType(p);
        if(pt == King)
        {
            return true;
        }
    }
    return false;
}

inline void Move::setPromotionPiece(PieceType type)
{
    m &= ~(7 << 22);
    m |= ((unsigned int) type & 7) << 22;
}

inline Square Move::from() const
{
    return m & 63;
}

inline Square Move::to() const
{
    return (m >> 6) & 63;
}

inline Move::Move()
    : m(0), u(0)
{}

inline Move::Move(const Square from, const Square to)
    : m(from | (to << 6)), u(0)
{}

inline Square Move::castlingRookFrom() const
{
    return (to() % 8 == 2) ? to() - 2 : to() + 1;
}

inline Square Move::castlingRookTo() const
{
    return (from() + to()) / 2;
}

inline QString Move::toAlgebraic() const
{
    if(!isLegal())
    {
        return QString("?");
    }
    QString alg;
    alg += QChar('a' + from() % 8);
    alg += QChar('1' + from() / 8);
    alg += QChar('a' + to() % 8);
    alg += QChar('1' + to() / 8);
    if (isPromotion())
    {
        alg += QChar('=');
        alg += "XKQRBNPKQRBNP"[promotedPiece()];
    }
    return alg;
}

inline Square Move::enPassantSquare() const
{
    return from() > 31 ? to() - 8 : to() + 8;
}

inline Piece Move::pieceMoved() const
{
    return  Piece((7 & (m >> 12)) + (m & BLACKTM ? 6 : 0));
}

inline Piece Move::capturedPiece() const
{
    uchar p = (m >> 18) & 7;
    if(p == 0)
    {
        return Piece(0);
    }
    return  Piece(p + (m & BLACKTM ? 0 : 6));
}

inline Piece Move::promotedPiece() const
{
    return Piece((7 & (m >> 22)) + (m & BLACKTM ? 6 : 0));
}

inline bool Move::isSpecial() const
{
    return m & SPECIALBITS;
}

inline bool Move::isPromotion() const
{
    return m & PROMOTEBIT;
}

inline bool Move::isCastling() const
{
    return m & CASTLINGBIT;
}

inline bool Move::isDoubleAdvance() const
{
    return m & TWOFORWARDBIT;
}

inline bool Move::isEnPassant() const
{
    return m & ENPASSANTBIT;
}

inline bool Move::isLegal() const
{
    return m & LEGALITYBIT;
}

inline Color Move::color() const
{
    return ((m & BLACKTM) ? Black : White);
}

inline void Move::genOneForward(unsigned int from, unsigned int to)
{
    m = from | (to << 6) | (Pawn << 12);
}

inline void Move::genTwoForward(unsigned int from, unsigned int to)
{
    m = from | (to << 6) | (Pawn << 12)   | (1 << 16);
}

inline void Move::genPromote(unsigned int from, unsigned int to, unsigned int type)
{
    m = from  | (to << 6) | (Pawn << 12)   | (type << 22)  | (1 << 17);
}

inline void Move::genCapturePromote(unsigned int from, unsigned int to, unsigned int type, unsigned int captured)
{
    m = from | (to << 6) | (Pawn << 12) | (captured << 18) | (type << 22) | (1 << 17);
}

inline void Move::genEnPassant(unsigned int from, unsigned int to)
{
    m = from  | (to << 6) | (Pawn << 12) | (Pawn << 18) | (1 << 21);
}

inline void Move::genPawnMove(unsigned int from, unsigned int to, unsigned int captured)
{
    m = from  | (to << 6) | (Pawn << 12) | (captured << 18);
}

inline void Move::genKnightMove(unsigned int from, unsigned int to, unsigned int captured)
{
    m = from  | (to << 6) | (Knight << 12) | (captured << 18);
}

inline void Move::genBishopMove(unsigned int from, unsigned int to, unsigned int captured)
{
    m = from  | (to << 6) | (Bishop << 12) | (captured << 18);
}

inline void Move::genRookMove(unsigned int from, unsigned int to, unsigned int captured)
{
    m = from  | (to << 6) | (Rook << 12)   | (captured << 18);
}

inline void Move::genQueenMove(unsigned int from, unsigned int to, unsigned int captured)
{
    m = from  | (to << 6) | (Queen << 12)  | (captured << 18);
}

inline void Move::genKingMove(unsigned int from, unsigned int to, unsigned int captured)
{
    m = from  | (to << 6) | (King << 12)   | (captured << 18);
}

inline void Move::genWhiteOO()
{
    m = 4 | (6 << 6) | (King << 12)  | (1 << 15);
}

inline void Move::genWhiteOOO()
{
    m = 4 | (2 << 6) | (King << 12)  | (1 << 15);
}

inline void Move::genBlackOO()
{
    m = 60 | (62 << 6) | (King << 12)  | (1 << 15);
}

inline void Move::genBlackOOO()
{
    m = 60 | (58 << 6) | (King << 12)  | (1 << 15);
}

inline void Move::setLegalMove()
{
    m |= LEGALITYBIT;
}

inline void Move::setFrom(Square from)
{
    m = (m & (~63)) | from;
    m &= ~LEGALITYBIT;
}

inline void Move::setTo(Square to)
{
    m = (m & (~(63 << 6))) | (to << 6);
    m &= ~LEGALITYBIT;
}

inline unsigned int Move::action() const
{
    return (m >> 12) & 63;
}

inline unsigned int Move::removal() const
{
    return (m >> 18) & 15;
}

inline void Move::setBlack()
{
    m |= BLACKTM;
}

inline unsigned int Move::promoted() const
{
    return 7 & (m >> 22);
}

inline unsigned int Move::capturedType() const
{
    return (m >> 18) & 7;
}

inline void Move::setPieceType(const uchar p)
{
    m &= PTCLEAR ;
    m |= (7 & p) << 12;
}

inline void Move::setCaptureType(const uchar p)
{
    m &= CAPCLEAR ;
    m |= (7 & p) << 18;
}

inline void Move::setTwoForward()
{
    m |= TWOFORWARDBIT;
}

inline void Move::setEnPassant()
{
    m |= ENPASSANTBIT;
    setCaptureType(Pawn);
}

inline void Move::setPromoted(const uchar p)
{
    m &= PROCLEAR ;
    m |= PROMOTEBIT | ((7 & p) << 22);
}

inline void Move::setCheck()
{
    m |= (1 << 30);
}

inline void Move::setMate()
{
    m |= (1 << 29);
}

inline bool operator==(const Move& m1, const Move& m2)
{
    return m1.m == m2.m;
}

inline bool operator<(const Move& m1, const Move& m2)
{
    return m1.m < m2.m;
}

inline bool Move::operator==(const Color& color) const
{
    return color == ((m & BLACKTM) ? Black : White);
}

inline bool Move::operator!=(const Color& color) const
{
    return !(*this == color);
}

inline bool Move::operator==(const Piece& p) const
{
    return (unsigned int) p == (((m & BLACKTM) && ((m >> 12) & 7)) ? ((m >> 12) & 7) + 6 : ((m >> 12) & 7));
}

inline bool Move::operator!=(const Piece& p) const
{
    return !(*this == p);
}

#endif // __MOVE_H__
