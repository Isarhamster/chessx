/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef PIECE_H
#define PIECE_H

enum Piece
{
    Empty,
    WhiteKing, WhiteQueen, WhiteRook, WhiteBishop, WhiteKnight, WhitePawn,
    BlackKing, BlackQueen, BlackRook, BlackBishop, BlackKnight, BlackPawn,
    ConstPieceTypes,
    InvalidPiece = 255
};

// Pre-Increment
Piece& operator++(Piece& w);

// Post-Increment
Piece operator++(Piece& w, int);

enum PieceType { None, King, Queen, Rook, Bishop, Knight, Pawn };

enum Color {White, Black, NoColor};

/** Return opposite color */
inline Color oppositeColor(Color c)
{
    if (c == White) return Black;
    else if (c == Black) return White;
    return NoColor;
}

/** Return true if given piece is White */
inline bool isWhite(Piece p)
{
    return p >= WhiteKing && p <= WhitePawn;
}

/** Return true if given piece is Black */
inline bool isBlack(Piece p)
{
    return p >= BlackKing && p <= BlackPawn;
}

/** Return PieceType of given Piece */
inline PieceType pieceType(Piece p)
{
    return PieceType(isWhite(p) ? p : (isBlack(p) ? p - 6 : 0));
}

/** Return Color of given Piece */
inline Color pieceColor(Piece p)
{
    return isBlack(p) ? Black : White;
}

/** Verify that given Piece is within range */
inline bool isValidPiece(Piece p)
{
    return isWhite(p) || isBlack(p);
}

#endif // PIECE_H

