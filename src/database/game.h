/***************************************************************************
                          game.h - chess game
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>
                           (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __GAME_H__
#define __GAME_H__

#include <qstring.h>

#include "board.h"
#include "movelist.h"
#include "partialdate.h"

/**
   The Game class represents a chess game. Moves and variations can be added
	 and removed. Moves can have associated comments and nag values. For methods
	 that accept a variation number 0 is the main line, with 1 and above being the
	 alternative lines.
*/

class Game
{
	friend class PgnExporter;
	public:
		//constructors
		/** Creates a game with no moves and a standard start position. */
		Game();
		~Game();

		//node information methods
		/** @return current position */
		Board board() const;
		/** @return first move in given variation */
		Move move(int variation = 0) const;
		/** @return comment associated with the first move in the given variation */
		QString annotation(int variation = 0) const;
		/** @return nag associated with the first move in the given variation */
		int nag(int variation = 0) const;
		
		//node modification methods
		/** Sets the comment associated with the first move in the given variation */
		bool setAnnotation(QString annotation, int variation = 0);
		/** Sets the nag associated with the first move in the given variation */
		bool setNag(int nag, int variation = 0);

		//tree information methods
		/** @return whether the game is currently at the start position */
		bool atStart() const;
		/** @return whether the game is at the end of the current variation */
		bool atEnd() const;
		/** @return number of half moves made since the beginning of the game */
		int ply() const;
		/** @return number of ply in current variation */
		int plyCount() const;
		/** @return number of current variation relative to previous move*/
		int variation() const;
		/** @return number of variations at the current position */
		int variationCount() const;
		
		//tree traversal methods
		/** Moves to the begining of the game */
		void toStart();
		/** Moves to the end of the current variation */
		void toEnd();
		/** Move forward the given number of moves, returns actual number of moves made */
		int forward(int count = 1);
		/** Move back the given number of moves, returns actual number of moves undone */
		int backward(int count = 1);
		/** Enters the given variation, returns if successful */
		bool enterVariation(int variation);
		/** Exits the current variation */
		void exitVariation();
		
		//tree modification methods
		/** Adds a move at the current position, returns variation number of newly added move */
		int addMove(const Move& move, const QString& annotation = QString::null, int nag = 0);
		/** Adds a move at the current position, returns variation number of newly added move */
		int addMove(const QString& sanMove, const QString& annotation = QString::null, int nag = 0);
		/** Promotes the given variation to the main line, returns true if sucessful */
		bool promoteVariation(int variation);
		/** Removes the next move from the given variation, returns true if sucessful */
		bool removeMove(int variation = 0);
		
		//game information methods
		/** @return tag id of white player */
		int white() const;
		/** @return tag id of black player */
		int black() const;
		/** @return date of game */
		PartialDate date() const;
		/** @return game event */
		int event() const;
		/** @return game site */
		int site() const;
		/** @return game round */
		int round() const;
		
		/** @return start position of game */
		Board startBoard() const;
		/** @return annotation at start of game */
		QString startAnnotation() const;
		/** @return game result */
		Result result() const;

		//game modification methods
		/** Sets white player */
		void setWhite(int tag);
		/** Sets black player */
		void setBlack(int tag);
		/** Sets game date */
		void setDate(const PartialDate& date);
		/** Sets game event */
		void setEvent(int tag);
		/** Sets game site */
		void setSite(int tag);
		/** Sets game round */
		void setRound(int tag);
		
		/** Sets the games start position */
		void setStartBoard(const Board& startBoard);
		/** Sets annotaions at the start of the game */
		void setStartAnnotation(const QString& annotation);
		/** Sets the game result */
		void setResult(const Result result);
		
	private:
		//definitions
		static const int defaultSize = 1000;
		
		struct MoveNode {
			Move move;
			QString annotation;
			int nag;
			int previousNode;
			int nextNode;
			int parentNode;
			int nextVariation;
		};

		//memory  management methods
		/** Compacts nodes and allocates more space if reqiured */
		void compact();
		/** Copys a variation, used by compact for copying nodes to new storage */
		void copyVariation(int parentNode, int startNode, MoveNode* destinationNodes);
		/** Counts number of descendat nodes, for recording number of deleted nodes */
		int countNodes(int node);
	
		//game data
		int m_white;
		int m_black;
		PartialDate m_date;
		int m_event;
		int m_site;
		int m_round;
		Board m_startBoard;
		QString m_startAnnotation;
		Result m_result;
		
		//tree data
		int m_currentNode;
		int m_ply;
		Board m_currentBoard;
		History m_history;
		
		int m_nextFreeNode;
		int m_deletedNodeCount;
		int m_totalNodeCount;
		MoveNode* m_moveNodes;
};

#endif	// __GAME_H__
