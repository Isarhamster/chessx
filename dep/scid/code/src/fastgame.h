/*
* Copyright (C) 2013-2015  Fulvio Benini

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

#ifndef FASTGAME_H
#define FASTGAME_H

#include "bytebuf.h"
#include "common.h"
#include "fullmove.h"
#include "movegen.h"
#include "position.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

/// Store the number of pieces for each type and color.
class MaterialCount {
	int8_t n_[2][8] = {};

public:
	/// Add one piece.
	void incr(colorT color, pieceT piece_type) {
		ASSERT(color == 0 || color == 1);
		ASSERT(piece_type > 0 && piece_type < 8);

		++n_[color][0];
		++n_[color][piece_type];
	}

	/// Subtract one piece.
	void decr(colorT color, pieceT piece_type) {
		ASSERT(color == 0 || color == 1);
		ASSERT(piece_type > 0 && piece_type < 8);

		--n_[color][0];
		--n_[color][piece_type];
	}

	/// Return the total number of pieces of the specified color.
	int8_t count(colorT color) const {
		ASSERT(color == 0 || color == 1);

		return n_[color][0];
	}

	/// Return the number of pieces of the specified color and type.
	int8_t count(colorT color, pieceT piece_type) const {
		ASSERT(color == 0 || color == 1);
		ASSERT(piece_type > 0 && piece_type < 8);

		return n_[color][piece_type];
	}

	bool operator==(const MaterialCount& b) const {
		const int8_t* a = n_[0];
		const int8_t* b_ptr = b.n_[0];
		return std::equal(a, a + 16, b_ptr);
	}

	bool operator!=(const MaterialCount& b) const {
		return !operator==(b);
	}
};

/// Store the type and position of the pieces compatibly with the SCID4 coding.
class PieceList {
	struct {
		squareT sq;
		pieceT piece_type;
	} pieces_[2][16];

public:
	/// SCID4 encoded games must use index 0 for kings.
	int8_t getKingIdx() const { return 0; }

	/// Return the type of the piece with index @e idx
	pieceT getPieceType(colorT color, int idx) const {
		ASSERT(color == 0 || color == 1);
		ASSERT(idx >= 0 && idx < 16);

		return pieces_[color][idx].piece_type;
	}

	/// Return the square position of the piece with index @e idx
	squareT getSquare(colorT color, int idx) const {
		ASSERT(color == 0 || color == 1);
		ASSERT(idx >= 0 && idx < 16);

		return pieces_[color][idx].sq;
	}

	/// Change the square position of the piece with index @e idx
	void changeSq(colorT color, int idx, squareT to) {
		ASSERT(color == 0 || color == 1);
		ASSERT(idx >= 0 && idx < 16);

		pieces_[color][idx].sq = to;
	}

	/// Change the type of the piece with index @e idx
	void promote(colorT color, int idx, pieceT piece_type) {
		ASSERT(color == 0 || color == 1);
		ASSERT(idx >= 0 && idx < 16);

		pieces_[color][idx].piece_type = piece_type;
	}

	/// Remove the piece with index @e removed_idx.
	/// Piece's indexes are important for decoding SCID4 moves:  when a piece is
	/// removed it's index is used by the last valid index @e lastvalid_idx.
	/// Return the square of the new piece with index @e removed_idx.
	squareT remove(colorT color, int removed_idx, int lastvalid_idx) {
		ASSERT(color == 0 || color == 1);
		ASSERT(removed_idx >= 0 && removed_idx < 16);
		ASSERT(lastvalid_idx >= 0 && lastvalid_idx < 16);

		pieces_[color][removed_idx] = pieces_[color][lastvalid_idx];
		return pieces_[color][lastvalid_idx].sq;
	}

	/// Set the type and square of the piece with index @e idx
	void set(colorT color, int idx, squareT sq, pieceT piece_type) {
		ASSERT(color == 0 || color == 1);
		ASSERT(idx >= 0 && idx < 16);
		ASSERT(piece_type != KING || idx == getKingIdx());

		pieces_[color][idx].sq = sq;
		pieces_[color][idx].piece_type = piece_type;
	}
};

class FastBoard {
	uint8_t board_[64];
	MaterialCount mt_;
	PieceList pieces_;
	uint8_t castlingRook_[2][2]; // [WHITE|BLACK][long|short] the idx of the
	                             // rooks that can castle. 0 if none

	enum { EMPTY_SQ_ = 0xFF };

public:
	FastBoard() = default;
	explicit FastBoard(const Position& pos) { Init(pos); }

	static FastBoard stdStart() {
		static const auto std_start = FastBoard(Position::getStdStart());
		return std_start;
	}

	void Init(const Position& pos) {
		std::fill_n(board_, 64, EMPTY_SQ_);
		std::fill_n(*castlingRook_, 4, 0);

		for (auto color : {WHITE, BLACK}) {
			const auto pos_count = pos.GetCount(color);
			const auto pos_list = pos.GetList(color);
			for (uint8_t idx = 0; idx < 16; ++idx) {
				if (idx < pos_count) {
					const squareT sq = pos_list[idx];
					const pieceT piece_type = piece_Type(pos.GetPiece(sq));
					pieces_.set(color, idx, sq, piece_type);
					board_[sq] = idx;
					mt_.incr(color, piece_type);

					if (piece_type == ROOK &&
					    square_Rank(sq) == rank_Relative(color, RANK_1)) {
						auto oldIdx = castlingRook_[color][0];
						if (!oldIdx || sq < pieces_.getSquare(color, oldIdx))
							castlingRook_[color][0] = idx;

						oldIdx = castlingRook_[color][1];
						if (!oldIdx || sq > pieces_.getSquare(color, oldIdx))
							castlingRook_[color][1] = idx;
					}
				} else {
					pieces_.set(color, idx, 0, INVALID_PIECE);
				}
			}
		}
	}

	bool isEqual(const pieceT* board, const MaterialCount& mt_count) const {
		if (mt_ != mt_count)
			return false;

		for (int idx = 0, n = mt_.count(WHITE); idx < n; ++idx) {
			const auto sq = pieces_.getSquare(WHITE, idx);
			const auto pt = pieces_.getPieceType(WHITE, idx);
			if (board[sq] != piece_Make(WHITE, pt))
				return false;
		}
		for (int idx = 0, n = mt_.count(BLACK); idx < n; ++idx) {
			const auto sq = pieces_.getSquare(BLACK, idx);
			const auto pt = pieces_.getPieceType(BLACK, idx);
			if (board[sq] != piece_Make(BLACK, pt))
				return false;
		}
		return true;
	}

	const MaterialCount& materialCount() const {
		return mt_;
	}

	squareT getSquare(colorT color, int idx) const {
		return pieces_.getSquare(color, idx);
	}

	pieceT getPiece(colorT color, int idx) const {
		return pieces_.getPieceType(color, idx);
	}

	/// The king and the rook are moved to the castle squares (even if they were
	/// no longer in their starting squares).
	/// @returns the previous position of the rook on success.
	///          On error returns the king's square (no piece is moved).
	template <colorT color> squareT castle(bool king_side) {
		const squareT king_to = king_side ? square_Relative(color, G1)
		                                  : square_Relative(color, C1);
		const squareT rook_to = king_side ? square_Relative(color, F1)
		                                  : square_Relative(color, D1);
		const int king_idx = pieces_.getKingIdx();
		const squareT king_from = pieces_.getSquare(color, king_idx);
		const int rook_idx = castlingRook_[color][king_side ? 1 : 0];
		const squareT rook_from = pieces_.getSquare(color, rook_idx);

		if (pieces_.getPieceType(color, rook_idx) != ROOK)
			return king_from; // No rook or captured

		pieces_.changeSq(color, rook_idx, rook_to);
		pieces_.changeSq(color, king_idx, king_to);
		board_[rook_from] = EMPTY_SQ_;
		board_[king_from] = EMPTY_SQ_;
		board_[rook_to] = rook_idx;
		board_[king_to] = king_idx;
		return rook_from;
	}

	template <colorT color>
	pieceT move(uint8_t idx, squareT to, pieceT promo) {
		if (promo != INVALID_PIECE) {
			pieces_.promote(color, idx, promo);
			mt_.incr(color, promo);
			mt_.decr(color, PAWN);
		}
		const auto from = pieces_.getSquare(color, idx);
		board_[from] = EMPTY_SQ_;
		pieces_.changeSq(color, idx, to);
		return remove<1 - color>(to, idx);
	}

	template <colorT color>
	pieceT remove(squareT sq, uint8_t newIdx = EMPTY_SQ_) {
		const uint8_t oldIdx = board_[sq];
		board_[sq] = newIdx;
		if (oldIdx == EMPTY_SQ_)
			return INVALID_PIECE;

		pieceT removed_pt =  pieces_.getPieceType(color, oldIdx);
		mt_.decr(color, removed_pt);
		int lastvalid_idx = mt_.count(color);
		if (oldIdx != lastvalid_idx) {
			squareT moved_sq = pieces_.remove(color, oldIdx, lastvalid_idx);
			board_[moved_sq] = oldIdx;

			for (auto& cRook : castlingRook_[color]) {
				if (cRook == lastvalid_idx)
					cRook = oldIdx;
			}
		}
		return removed_pt;
	}

	/**
	 * Given the actual board position, find if the last move needs to be made
	 * unambiguous and if it gives check (or TODO mate), and then sets the
	 * appropriate bits in @e lastmove.
	 * @param lastmove: the last move played.
	 */
	void fillSANInfo(FullMove& lastmove) const {
		const auto lastFrom = lastmove.getFrom();
		const auto lastTo = lastmove.getTo();
		const auto lastCol = lastmove.getColor();
		auto lastPt = lastmove.getPiece();

		if (lastPt == PAWN) {
			if (lastmove.isPromo())
				lastPt = lastmove.getPromo();
		} else if (mt_.count(lastCol, lastPt) > 1) {
			int ambiguity = ambiguousMove(lastFrom, lastTo, lastCol, lastPt);
			if (ambiguity)
				lastmove.setAmbiguity(ambiguity != 5, ambiguity >= 5);
		}

		// Look for checks
		ASSERT(mt_.count(WHITE) >= 1 && mt_.count(BLACK) >= 1);

		auto isOccupied = [this](auto sq) { return board_[sq] != EMPTY_SQ_; };
		const auto enemyKingSq = getKingSquare(color_Flip(lastCol));
		bool direct_check = (lastPt != KING) &&
		                    movegen::attack(lastTo, enemyKingSq, lastCol,
		                                    lastPt, isOccupied);
		if (direct_check || // Look for a discovered check
		    find_attacker_slider(enemyKingSq, lastCol) >= 0) {
			lastmove.setCheck();

			// TODO: Find if it's mate:
			// - it's not mate if the king can move to a safe square
			// - it's mate if it's double check or the attacker cannot be
			//   captured or blocked.
		}
	}

private:
	squareT getKingSquare(colorT color) const {
		return pieces_.getSquare(color, pieces_.getKingIdx());
	}

	int ambiguousMove(squareT lastFrom, squareT lastTo, colorT lastCol,
	                  pieceT lastPt) const {
		int ambiguity = 0;

		const squareT kingSq = getKingSquare(lastCol);
		const colorT enemyCol = color_Flip(lastCol);
		for (int i = 1, n = mt_.count(lastCol); i < n; i++) {
			if (getPiece(lastCol, i) != lastPt)
				continue; // Skip: different type

			const squareT sq = getSquare(lastCol, i);
			if (sq == lastTo)
				continue; // Skip: this is the analyzed piece

			auto isOccupied = [sq, lastFrom, this](auto square) {
				if (square == sq)
					return false;
				if (square == lastFrom)
					return true;
				return board_[square] != EMPTY_SQ_;
			};
			if (!movegen::pseudo(sq, lastTo, lastCol, lastPt, isOccupied))
				continue; // Skip: illegal move

			const auto pin = movegen::opens_ray(sq, lastTo, kingSq, isOccupied);
			if (pin.first != INVALID_PIECE) {
				uint8_t idx = board_[pin.second];
				if (idx != EMPTY_SQ_ && idx < mt_.count(enemyCol) &&
				    getSquare(enemyCol, idx) == pin.second) {
					const pieceT pt = getPiece(enemyCol, idx);
					if (pt == QUEEN || pt == pin.first)
						continue; // Skip: pinned piece
				}
			}

			// Ambiguity:
			// 1 (0001) --> need from-file (preferred) or from-rank
			// 3 (0011) --> need from-file
			// 5 (0101) --> need from-rank
			// 7 (0111) --> need both from-file and from-rank
			ambiguity |= 1;
			if (square_Rank(lastFrom) == square_Rank(sq)) {
				ambiguity |= 2; // 0b0010
			} else if (square_Fyle(lastFrom) == square_Fyle(sq)) {
				ambiguity |= 4; // 0b0100
			}
		}

		return ambiguity;
	}

	int find_attacker_slider(squareT destSq, colorT color) const {
		for (int idx = 0, n = mt_.count(color); idx < n; ++idx) {
			const pieceT pt = getPiece(color, idx);
			if (pt != QUEEN && pt != ROOK && pt != BISHOP)
				continue;

			auto isOccupied = [this](auto square) {
				return board_[square] != EMPTY_SQ_;
			};
			const squareT sq = getSquare(color, idx);
			if (movegen::attack_slider(sq, destSq, pt, isOccupied)) {
				return idx;
			}
		}
		return -1;
	}
};

class GameView {
	FastBoard board_;
	ByteBuffer bbuf_;
	colorT cToMove_;

public:
	explicit GameView(const ByteBuffer& bbuf)
	    : board_(FastBoard::stdStart()), bbuf_(bbuf), cToMove_(WHITE) {}

	GameView(const ByteBuffer& bbuf, const Position& startPos) : bbuf_(bbuf) {
		board_.Init(startPos);
		cToMove_ = startPos.GetToMove();
	}

	template <typename FuncT> void mainLine(FuncT fn) {
		while (const auto move = (cToMove_ == WHITE)
		                             ? DecodeNextMove<FullMove, WHITE>()
		                             : DecodeNextMove<FullMove, BLACK>()) {
			cToMove_ = 1 - cToMove_;
			if (!fn(move))
				return;
		}
	}

	FullMove getMove(int ply_to_skip) {
		for (int ply=0; ply <= ply_to_skip; ply++, cToMove_ = 1 - cToMove_) {
			auto move = (cToMove_ == WHITE)
			                ? DecodeNextMove<FullMove, WHITE>()
			                : DecodeNextMove<FullMove, BLACK>();
			if (!move)
				break;

			if (ply == ply_to_skip) {
				board_.fillSANInfo(move);
				cToMove_ = 1 - cToMove_;
				return move;
			}
		}
		return {};
	}

	std::string getMoveSAN(int ply_to_skip, int count) {
		std::stringstream res;
		const auto ply_num = (cToMove_ == WHITE)? 2 : 3;
		for (int ply=0; ply < ply_to_skip + count; ply++, cToMove_ = 1 - cToMove_) {
			FullMove move;
			if (cToMove_ == WHITE) {
				move = DecodeNextMove <FullMove, WHITE>();
				if (!move)
					break;
				if (ply < ply_to_skip)
					continue;
				if (ply > ply_to_skip)
					res << "  ";
				res << (ply_num + ply) / 2 << ".";
			} else {
				move = DecodeNextMove <FullMove, BLACK>();
				if (!move)
					break;
				if (ply < ply_to_skip)
					continue;
				if (ply == ply_to_skip)
					res << (ply_num + ply) / 2 << "...";
				else
					res << " ";
			}
			board_.fillSANInfo(move);
			res << move.getSAN();
		}
		return res.str();
	}

	template <colorT toMove>
	int search(const byte* board, const MaterialCount& mt_count) {
		int ply = 1;
		auto less_material = [](const MaterialCount& a, const MaterialCount& b,
		                        const colorT color, const auto move) {
			if (!move)
				return true;

			const auto captured_pt = move.getCaptured();
			if (captured_pt == INVALID_PIECE)
				return false;

			if (a.count(color) < b.count(color))
				return true;

			return a.count(color, PAWN) + a.count(color, captured_pt) <
			       b.count(color, PAWN) + b.count(color, captured_pt);
		};

		if (cToMove_ != toMove) {
			const auto move = DecodeNextMove<FullMove, 1 - toMove>();
			if (!move)
				return 0;
			ply += 1;
		}
		for (;;) {
			if (board_.isEqual(board, mt_count))
				return ply;

			{
				const auto move = DecodeNextMove<FullMove, toMove>();
				if (less_material(board_.materialCount(), mt_count, 1 - toMove,
				                  move))
					return 0;
			}
			{
				const auto move = DecodeNextMove<FullMove, 1 - toMove>();
				if (less_material(board_.materialCount(), mt_count, toMove,
				                  move))
					return 0;
			}

			ply += 2;
		}
		return 0;
	}

private:
	template <typename TResult, colorT toMove> TResult DecodeNextMove() {
		auto [err, val] = bbuf_.nextLineMove();
		if (err)
			return {};

		return doPly<TResult, toMove>(val);
	}

	template <typename TResult, colorT toMove> TResult doPly(byte v) {
		const auto idx_piece_moving = v >> 4;
		pieceT moving_piece = board_.getPiece(toMove, idx_piece_moving);
		squareT from = board_.getSquare(toMove, idx_piece_moving);

		auto [to, promo] = bbuf_.decodeMove<toMove>(moving_piece, from, v);
		if (to < 0 || to > 63)
			return {}; // decode error

		if (to == from) {
			if (promo == INVALID_PIECE)
				return {}; // decode error

			if (promo == PAWN) // NULL MOVE
				return TResult(toMove, 0, 0, KING);

			// CASTLE
			const squareT rook_from = board_.castle<toMove>(promo == KING);
			if (rook_from == from)
				return {}; // decode error
			return TResult(toMove, from, rook_from);
		}

		bool enPassant = moving_piece == PAWN &&
		                 square_Fyle(from) != square_Fyle(to);
		pieceT captured = board_.move<toMove>(idx_piece_moving, to, promo);
		TResult res(toMove, from, to, moving_piece);
		if (promo != INVALID_PIECE)
			res.setPromo(promo);
		if (captured != INVALID_PIECE) {
			res.setCapture(captured, false);
		} else if (enPassant) {
			squareT sq = (toMove == WHITE) ? to - 8 : to + 8;
			captured = board_.remove<1 - toMove>(0x3F & sq);
			res.setCapture(captured, true);
		}
		return res;
	}
};


#endif
