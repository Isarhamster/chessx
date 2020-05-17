/*
 * Copyright (C) 2011-2019  Fulvio Benini
 *
 * This file is part of Scid (Shane's Chess Information Database).
 *
 * Scid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 *
 * Scid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Scid.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 * Implements functions for the validation of chess moves.
 */

#include <utility>

// These functions use the following move classification:
// - a VALID move is a move that respects the basic rules for moving the pieces,
//   for example a bishop should move diagonally. Validating this type of moves
//   do not require any info about the current position.
// - an ATTACK move is a valid move that can capture an enemy piece at the
//   destination square. It takes into account the special rule for pawns that
//   can capture only diagonally and the board position for obstacles, for
//   example a valid diagonal bishop move is an attack move only if the
//   in-between squares are empty. To validate this type of moves it is
//   necessary to know the location of empty squares in the current position.
// - a PSEUDO-LEGAL move is an attack move, or a non-capture pawn move, or a
//   castle move.
// - a LEGAL move is a pseudo-legal move where the destination square is not
//   occupied by a friendly piece and that do not leave the king in check.
//   To validate this type of moves it is necessary to know the type and
//   position of every piece.

namespace movegen {

const int NSQUARES = 8;

inline bool valid_king(squareT sqFrom, squareT sqTo) {
	unsigned distRank = 1 + (sqTo / NSQUARES) - (sqFrom / NSQUARES);
	unsigned distFyle = 1 + (sqTo % NSQUARES) - (sqFrom % NSQUARES);
	return distRank <= 2 && distFyle <= 2;
}

inline bool valid_knight(squareT sqFrom, squareT sqTo) {
	int distRank = (sqTo / NSQUARES) - (sqFrom / NSQUARES);
	int distFyle = (sqTo % NSQUARES) - (sqFrom % NSQUARES);
	int distProduct = distRank * distFyle;
	return (distProduct == 2 || distProduct == -2);
}

inline int valid_slider(squareT sqFrom, squareT sqTo, pieceT pieceType) {
	ASSERT(pieceType == QUEEN || pieceType == ROOK || pieceType == BISHOP);

	int distRank = (sqTo / NSQUARES) - (sqFrom / NSQUARES);
	int distFyle = (sqTo % NSQUARES) - (sqFrom % NSQUARES);

	// Make sure the direction is valid:
	int sqStep;
	bool isDiagonal = false;
	if (distRank == 0) {
		sqStep = 1; // horizontal
	} else if (distFyle == 0) {
		sqStep = NSQUARES; // vertical
	} else if (distFyle == distRank) {
		sqStep = NSQUARES + 1;
		isDiagonal = true;
	} else if (distFyle == -distRank) {
		sqStep = NSQUARES - 1;
		isDiagonal = true;
	} else {
		return 0;
	}
	if (pieceType == ROOK && isDiagonal)
		return 0;
	if (pieceType == BISHOP && !isDiagonal)
		return 0;

	return sqStep;
}

inline bool attack_pawn(squareT sqFrom, squareT sqTo, colorT pieceCol) {
	int distRank = (sqTo / NSQUARES) - (sqFrom / NSQUARES);
	int distFyle = (sqTo % NSQUARES) - (sqFrom % NSQUARES);
	if (pieceCol == WHITE && distRank != 1)
		return false;
	if (pieceCol == BLACK && distRank != -1)
		return false;

	return (distFyle == 1 || distFyle == -1);
}

template <typename TFunc>
bool attack_slider(squareT sqFrom, squareT sqTo, pieceT pieceType,
                   TFunc isOccupied) {
	int sqStep = valid_slider(sqFrom, sqTo, pieceType);
	if (sqStep == 0)
		return false;

	// Make sure all the in-between squares are empty:
	if (sqFrom > sqTo)
		sqStep = -sqStep;

	for (int sq = sqFrom + sqStep; sq != sqTo; sq += sqStep) {
		if (isOccupied(sq))
			return false;
	}

	return true;
}

/**
 * Validate an ATTACK move, that is if a piece placed at @e sqFrom can capture
 * an enemy piece at @e sqTo.
 * @param sqFrom:       square of the piece.
 * @param sqTo:         square of the piece to be captured.
 * @param pieceCol:     color of the moving piece.
 * @param pieceType:    type of the moving piece.
 * @param isOccupied:   callable object which should returns true if a square is
 *                      occupied by a piece. Since it is not invoked with @e
 *                      sqFrom or @e sqTo, it's is irrelevant if the position is
 *                      the one before or after the move was made.
 * @returns true if the move is a valid ATTACK move.
 */
template <typename TFunc>
bool attack(squareT sqFrom, squareT sqTo, pieceT pieceCol, pieceT pieceType,
            TFunc isOccupied) {
	switch (pieceType) {
	case KING:
		return valid_king(sqFrom, sqTo);
	case KNIGHT:
		return valid_knight(sqFrom, sqTo);
	case PAWN:
		return attack_pawn(sqFrom, sqTo, pieceCol);
	default:
		break;
	}
	return attack_slider(sqFrom, sqTo, pieceType, isOccupied);
}

template <typename TFunc>
bool pseudo(squareT sqFrom, squareT sqTo, colorT /*pieceCol*/, pieceT pieceType,
            TFunc isOccupied) {
	// TODO: pawn and king moves
	ASSERT(pieceType != PAWN && pieceType != KING);

	switch (pieceType) {
	case KNIGHT:
		return valid_knight(sqFrom, sqTo);
	default:
		break;
	}
	return attack_slider(sqFrom, sqTo, pieceType, isOccupied);
}

/**
 * Given a pseudo-legal move, this functions return the type and the location of
 * the piece that can possibly pin the moving piece, making the move not legal.
 * @param sqFrom:       start square of the pseudo-legal move.
 * @param sqTo:         destination square of the pseudo-legal move.
 * @param sqRay:        the projected ray starts from @e sqRay and goes through
 *                      @e sqFrom; it is usually the square where the king is.
 * @param isOccupied:   callable object which should returns true if a square is
 *                      occupied by a piece.
 * @returns a std::pair with the type (INVALID_PIECE, BISHOP, ROOK) and the
 * square of the candidate pinning piece. If the type is INVALID_PIECE there is
 * no pin and the move is legal, otherwise it's necessary to test the board
 * position and if the returned square is occupied by an enemy QUEEN, or an
 * enemy piece matching the returned type, the move is not legal.
 */
template <typename TFunc>
inline std::pair<pieceT, squareT> opens_ray(squareT sqFrom, squareT sqTo,
                                            squareT sqRay, TFunc isOccupied) {
	ASSERT(sqRay != sqFrom);

	int fyleFrom = sqFrom % NSQUARES;
	int distFyle = (sqRay % NSQUARES) - fyleFrom;
	int distRank = (sqRay / NSQUARES) - (sqFrom / NSQUARES);

	// Make sure the direction is valid:
	int fyleEdge;
	int sqStep;
	pieceT pt;
	if (distFyle == 0) {
		sqStep = NSQUARES; // vertical
		fyleEdge = -1;
		pt = ROOK;
	} else {
		if (fyleFrom == 0 || fyleFrom == (NSQUARES - 1))
			return {INVALID_PIECE, 0};

		if (distRank == 0) {
			sqStep = 1; // horizontal
			fyleEdge = 0;
			pt = ROOK;
		} else if (distFyle == distRank) {
			sqStep = NSQUARES + 1;
			fyleEdge = 0;
			pt = BISHOP;
		} else if (distFyle == -distRank) {
			sqStep = NSQUARES - 1;
			fyleEdge = NSQUARES - 1;
			pt = BISHOP;
		} else {
			return {INVALID_PIECE, 0};
		}
	}
	if (sqFrom > sqRay) {
		sqStep = -sqStep;
		fyleEdge = NSQUARES - 1 - fyleEdge;
	}

	for (int sq = sqFrom + sqStep; sq != sqRay; sq += sqStep) {
		if (sq == sqTo || isOccupied(sq))
			return {INVALID_PIECE, 0};
	}

	for (int sq = sqFrom - sqStep; sq < NSQUARES * NSQUARES; sq -= sqStep) {
		if (sq < 0 || sq == sqTo)
			break;

		if (isOccupied(sq))
			return {pt, static_cast<squareT>(sq)};

		if ((sq % NSQUARES) == fyleEdge)
			break;
	}
	return {INVALID_PIECE, 0};
}

} // end of namespace movegen
