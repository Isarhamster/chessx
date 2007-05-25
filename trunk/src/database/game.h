/***************************************************************************
                          game.h - chess game
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>
                           (C) 2005, 2006 William Hoggarth <whoggarth@users.sourceforge.net>
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

#include <QMap>
#include <QString>

#include "board.h"
#include "movelist.h"
#include "partialdate.h"
#include "nag.h"

#define COMPILED_ECO_FILE_ID ((quint32)0xCD5CBD02U)

/** @ingroup Database
   The Game class represents a chess game. Moves and variations can be added
	 and removed. Moves can have associated comments and nag values. For methods
	 that accept a variation number 0 is the main line, with 1 and above being the
	 alternative lines.
*/

class Game
{
public:
	/**
	    Flags indicating how a move string should be constructed

	    These flags may be or-ed together to specify what should be included in
	    a move string.
	*/
	enum MoveStringFlags {
		MoveOnly = 0,     /**< Only the algebraic notation should be included */
		WhiteNumbers = 1, /**< White moves should be preceded by a move number */
		BlackNumbers = 2, /**< Black moves should be preceded by a move number */
		Nags = 4,         /**< Nags/symbolic annotation should be included */
		FullDetail = 7    /**< Specifies all move numbers and nags should be included */
	};

	enum {
		Start = -1000,
		PreviousMove = -1,
		NextMove = 1,
		End = 1000
	};
	//constructors
	/** Creates a game with no moves and a standard start position. */
	Game();
	Game(const Game& game);
	Game& operator=(const Game& game);
	~Game();

	//node information methods
	/** @return current position */
	const Board& board() const;
	/** @return current position in FEN */
	QString toFen() const;
	/** @return whether the current position is in the mainline */
	bool isMainline();
	/** @return first move in given variation */
	Move move(int variation = 0) const;
	/** @return integer id for move. Note that @p id is supposed to change after the game is modified. */
	int moveId(int variation = 0) const;
	/** @return current move id. */
	int currentMoveId() const;
	/** @return comment associated with the first move in the given variation */
	QString annotation(int variation = 0) const;
	/** @return nags associated with the first move in the given variation */
	NagSet nags(int variation = 0) const;
	/** @return previous move in short algebraic notation, returns empty string if no such move */
	QString previousMoveToSan(MoveStringFlags flags = MoveOnly);
	/** @return move in short algebraic notation */
	QString moveToSan(MoveStringFlags flags = MoveOnly, int variation = 0);

	//node modification methods
	/** Sets the comment associated with the first move in the given variation */
	bool setAnnotation(QString annotation, int variation = 0);
	/** Adds a nag to the first move in the given variation */
	bool addNag(Nag nag, int variation = 0);
	/** Sets the nags associated with the first move in the given variation */
	bool setNags(NagSet nags, int variation = 0);

	//tree information methods
	/** @return whether the game is currently at the start position */
	bool atStart() const;
	/** @return whether the game is at the end of the current variation */
	bool atEnd() const;
	/** Counts the number of moves, comments and nags, in all variations, to the end of the game */
	void moveCount(int* moves, int* comments, int* nags);
	/** @return number of half moves made since the beginning of the game */
	int ply() const;
	/** @return current move. Equals to @p ply/2 for standard games, but may be different
	*/
	int moveNumber() const;
	/** @return number of ply in current variation */
	int plyCount() const;
	/** @return number of current variation relative to previous move*/
	int currentVariation() const;
	/** @return number of variations at the current position (includes main line) */
	int variationCount() const;

	//tree traversal methods
	/** Moves to the beginning of the game */
	void moveToStart();
	/** Moves by given ply, returns actual ply reached */
	int moveByPly(int diff);
	/** Moves to the given ply, returns actual ply reached */
	int moveToPly(int ply);
	/** Moves to the position corresponding to the given move id */
	void moveToId(int moveId);
	/** Moves to the end of the current variation */
	void moveToEnd();
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
	int addMove(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet());
	/** Adds a move at the current position, returns variation number of newly added move */
	int addMove(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet());
	/** Replaces the next move in the given variation, returns true if successful */
	bool replaceMove(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet(), int variation = 0);
	/** Replaces the next move in the given variation, returns true if successful */
	bool replaceMove(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet(), int variation = 0);
	/** Promotes the given variation to the main line, returns true if successful */
	bool promoteVariation(int variation);
	/** Removes the given variation, returns true if successful */
	bool removeVariation(int variation = 0);
	/** Removes all variations and mainline moves after the current position */
	void truncateGameEnd();
	/** Truncates the game to the given variation, returns true is successful */
	bool truncateGameStart(int variation = 0);

	//game information methods
	/** @return ECO code for the game */
	QString ecoClassify() const;

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
	/** Removes all tags and moves */
	void clear();
	/** Removes all tags */
	void clearTags();
	/** @return value of the given tag */
	QString tag(const QString& tag) const;
	/** @return a map of all tags in the game */
	QMap<QString, QString> tags() const;
	/** Sets the value of the given tag */
	void setTag(const QString& tag, const QString& value);
	/** Removes a tag */
	void removeTag(const QString& tag);

	/** Set the game start position */
	void setStartBoard(const Board& startBoard, int firstMove = 1);
	/** Set the game start position from FEN. */
	void setStartBoard(const QString& fen);
	/** Set annotations at the start of the game */
	void setStartAnnotation(const QString& annotation);
	/** Set the game result */
	void setResult(const Result result);

	/** Class method that loads a file containing ECO classifications for use by the ecoClassify method. Returns true if successful */
	static bool loadEcoFile(const QString& ecoFile);

private:
	//definitions
	static const int defaultSize = 1000;

	struct MoveNode
	{
		Move move;
		QString annotation;
		NagSet nags;
		int previousNode;
		int nextNode;
		int parentNode;
		int nextVariation;
	};

	//memory  management methods
	/** Compacts nodes and allocates more space if required */
	void compact();
	/** Copies a variation, used by compact for copying nodes to new storage */
	void copyVariation(int parentNode, int startNode, MoveNode* destinationNodes, int endNode = 0);
	/** Counts the number of moves, comments and nags, in all variations, to the end of the game */
	void moveCount(int node, int* moves, int* comments, int* nags);
	/** Counts number of descendant nodes, for recording number of deleted nodes */
	int nodeCount(int node);
	//game data
	QMap<QString, QString> m_tags;
	Board m_startBoard;
	QString m_startAnnotation;
	Result m_result;

	//tree data
	int m_currentNode;
	int m_ply;
	int m_startPly; // Non-zero if boards doesn't start from starting position
	Board m_currentBoard;

	int m_nextFreeNode;
	int m_deletedNodeCount;
	int m_totalNodeCount;
	MoveNode* m_moveNodes;
	//eco data
	static QMap<quint64, QString> m_ecoPositions;
};

#endif	// __GAME_H__
