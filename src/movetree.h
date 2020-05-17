/*
 * Copyright (C) 2017  Fulvio Benini

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
 *
 */

/** @file
 * A tree graph of moves representing a game.
 */

#ifndef SCID_MOVETREE_H
#define SCID_MOVETREE_H

#include "common.h"
#include "movelist.h"
#include <string>

enum markerT : byte { NO_MARKER = 0, START_MARKER = 1, END_MARKER = 2 };

// MAX_NAGS is the maximum number of NAGs (annotation symbols) a single
// move can have:
#define MAX_NAGS 8

// This class creates a tree graph of moves representing a game.
//
// For each node there are two edges: *next* (to the next move) and *varChild*
// (to an alternative line). For example the graph for the move sequence
// "1.d4 (1.e4 e5 ( 1...c5)) (1.c4) 1...d5 2.c4" is:
// clang-format off
// (START_MARKER, pgn_1) --next-> (d4, pgn_2) --next-> (d5, pgn_10) --next-> (c4, pgn_11) --next-> (END_MARKER, none)
//                                      |
//                                      |-child-> (START_MARKER, pgn_3) --next-> (e4, pgn_4) --next-> (e5, pgn_5) --next-> (END_MARKER, none)
//                                                           |                                             |
//                                                           |                                             |-child-> (START_MARKER, pgn_6) --next-> (c5, pgn_7) --next-> (END_MARKER, none)
//                                                           |
//                                                           |-child-> (START_MARKER, pgn_8) --next-> (c4, pgn_9) --next-> (END_MARKER, none)
// clang-format on
// The graph illustrates also that:
// - each variation starts and ends with two nodes containing a START_MARKER
//   and a END_MARKER instead of a move. This implies that *next* never points
//   to a START_MARKER node and *varChild* always point to a START_MARKER.
// - multiple variations are stored as a list of children. In the example there
//   are two alternatives to "1.d4": "1.e4" is stored as direct child of "1.d4",
//   instead "1.c4" is stored as the child of the START_MARKER of "1.e4".
// - when represented as a PGN string, the moves creates a sequence that can be
//   counted (each new variation is counted as an additional move). This is used
//   to specify a location in the game.
//
struct moveT {
	simpleMoveT moveData; // piece moving, target square etc
	moveT* prev;
	moveT* next;
	moveT* varChild;
	char san[10];   // SAN representation of move
	markerT marker; // can be NO_MARKER, START_MARKER or END_MARKER
	byte numVariations;
	byte nagCount;
	byte nags[MAX_NAGS];
	std::string comment;

	void clear() {
		moveData = simpleMoveT();
		prev = nullptr;
		next = nullptr;
		varChild = nullptr;
		std::fill_n(san, sizeof(san), '\0');
		marker = NO_MARKER;
		numVariations = 0;
		nagCount = 0;
		std::fill_n(nags, sizeof(nags), 0);
		comment.clear();
	}

	bool startMarker() const { return marker == START_MARKER; }
	bool endMarker() const { return marker == END_MARKER; }
	bool isNull() const { return moveData.isNullMove(); }

	template <typename TNew>
	moveT* cloneLine(moveT* parent, TNew newMove) const {
		const moveT* orig = this;
		moveT* top = newMove();
		*top = *orig;
		top->prev = parent;
		parent = top;
		if (orig->varChild) {
			auto root = parent->startMarker() ? parent->prev : parent;
			parent->varChild = orig->varChild->cloneLine(root, newMove);
		}

		while (orig->next) {
			orig = orig->next;
			auto copy = newMove();
			*copy = *orig;
			parent->setNext(copy);
			parent = copy;
			if (orig->varChild) {
				auto root = parent->startMarker() ? parent->prev : parent;
				parent->varChild = orig->varChild->cloneLine(root, newMove);
			}
		}
		return top;
	}

	void swapLine(moveT& move) {
		ASSERT(prev && move.prev);
		ASSERT(!startMarker() && !move.startMarker());
		ASSERT(!endMarker() && !move.endMarker());
		// Swap lines
		auto swap_tmp = move.prev;
		prev->setNext(&move);
		swap_tmp->setNext(this);
		// Swap children
		std::swap(varChild, move.varChild);
		std::swap(numVariations, move.numVariations);
		auto updateParentLink = [](moveT& parent) {
			for (auto tmp = parent.varChild; tmp; tmp = tmp->varChild) {
				tmp->prev = &parent;
			}
		};
		updateParentLink(*this);
		updateParentLink(move);
	}

	const moveT* getPrevMove() const {
		if (!startMarker() && !prev->startMarker())
			return prev;

		auto root = getParent().first;
		return root ? root->getPrevMove() : nullptr;
	}

	std::pair<const moveT*, const moveT*> getParent() const {
		const moveT* varStart = this;
		while (!varStart->startMarker()) {
			varStart = varStart->prev;
		}
		return {varStart->prev, varStart};
	}

	std::pair<moveT*, moveT*> getParent() {
		moveT* varStart = this;
		while (!varStart->startMarker()) {
			varStart = varStart->prev;
		}
		return {varStart->prev, varStart};
	}

	moveT* nextMoveInPGN() const {
		if (endMarker()) {
			auto parent = getParent();
			auto nextVar = parent.second->varChild;
			return (nextVar) ? nextVar
			                 : (parent.first) ? parent.first->next : nullptr;
		}
		return (startMarker() || !varChild) ? next : varChild;
	}

	void setNext(moveT* move) {
		ASSERT(move);
		this->next = move;
		move->prev = this;
	}

	void insertChild(moveT* varStart, int pos) {
		ASSERT(!this->startMarker() && !this->endMarker());
		ASSERT(this != varStart);
		ASSERT(varStart->startMarker());
		auto parent = this;
		for (; parent->varChild && pos != 0; --pos) {
			parent = parent->varChild;
		}
		varStart->varChild = varStart;
		std::swap(parent->varChild, varStart->varChild);

		varStart->prev = this;
		++numVariations;
	}

	void appendChild(moveT* varStart) { insertChild(varStart, -1); }

	void detachChild(moveT* varStart) {
		ASSERT(this != varStart);
		ASSERT(varStart->startMarker());
		auto parent = this;
		while (parent->varChild != varStart) {
			parent = parent->varChild;
		}
		std::swap(parent->varChild, varStart->varChild);
		--numVariations;
	};
};

#endif // SCID_MOVETREE_H
