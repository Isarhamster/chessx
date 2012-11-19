/***************************************************************************
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"
#include "movelist.h"
#include "nag.h"

#define NO_MOVE -1
#define CURRENT_MOVE -2
#define CURRENT_VARIATION -3
#define COMPILED_ECO_FILE_ID ((quint32)0xCD5CBD02U)

typedef int MoveId;

/** @ingroup Core
   The Game class represents a chess game. This is a complete rewrite, with simpler
   API.  Moves and variations can be added and removed.
   Moves can have associated comments and nag values. Each variation can have a
   comment before the variation. The game can also have a pre-comment.

   Each node has a unique id that identifies it. Each variation is identified
   by the first node of the variation.

   Representation of a game tree
   0   1   2   3   4   5   6
   *---*---*---*---*---*---*
            \
             \ 7   8   9   10  11
               *---*---*---*---*
   0 is the parent node of the game. It does not represent a move
   1 represents the first move in the mainline.
   2 is the parent node of the first variation
   7 is the variation number.

*/

class Game
{
public :
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
	enum Position {
		BeforeMove,
		AfterMove
	};
	enum NextPreviousMove {
		NextMove,
		PreviousMove
	};
	Game();
	Game(const Game& game);
	Game& operator=(const Game& game);
	~Game();
	// **** Querying game information ****
    /** @return current position */
	const Board& board() const;
	/** @return current position in FEN */
	QString toFen() const;
	/** @return whether the current position is in the mainline */
	bool isMainline(MoveId moveId = CURRENT_MOVE) const;
	/** @return start position of game */
	Board startingBoard() const;
	/** @return game result */
	Result result() const;
	/** @return the move at node @p moveId. */
	Move move(MoveId moveId = CURRENT_MOVE) const;
	/** @return current move id. */
	MoveId currentMove() const;
	/** @return comment at move at node @p moveId. */
	QString annotation(MoveId moveId = CURRENT_MOVE, Position position = AfterMove) const;

    /** @return squareAnnotation at move at node @p moveId. */
    QString squareAnnotation(MoveId moveId = CURRENT_MOVE) const;
    /** @return arrowAnnotation at move at node @p moveId. */
    QString arrowAnnotation(MoveId moveId = CURRENT_MOVE) const;
    /** @return annotation at move at node @p moveId. */
    QString egtAnnotation(MoveId moveId = CURRENT_MOVE) const;
    /** @return annotation at move at node @p moveId. */
    QString clkAnnotation(MoveId moveId = CURRENT_MOVE) const;

    /** @return @p true if a move can have annotation before the move. */
	bool canHaveStartAnnotation(MoveId moveId = CURRENT_MOVE) const;
	/** @return nags for move at node @p moveId */
	NagSet nags(MoveId moveId = CURRENT_MOVE) const;
	/** @return next move in short algebraic notation, returns empty string if no such move */
	QString moveToSan(MoveStringFlags flags = MoveOnly,
			  NextPreviousMove nextPrevious = NextMove, MoveId moveId = CURRENT_MOVE);
    /** return comment associated with game */
    QString gameComment() const;

	// **** node modification methods ****
	/** Sets the comment associated with move at node @p moveId */
    bool setAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE, Position position = AfterMove);

    /** Sets the squareAnnotation associated with move at node @p moveId */
    bool setSquareAnnotation(QString squareAnnotation, MoveId moveId = CURRENT_MOVE);

    /** Sets the arrowAnnotation associated with move at node @p moveId */
    bool setArrowAnnotation(QString arrowAnnotation, MoveId moveId = CURRENT_MOVE);

    /** Sets the annotation associated with move at node @p moveId */
    bool setEgtAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE);
    /** Sets the annotation associated with move at node @p moveId */
    bool setClkAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE);

    /** Adds a nag to move at node @p moveId */
	bool addNag(Nag nag, MoveId moveId = CURRENT_MOVE);
	/** Sets nags for move at node @p moveId */
	bool setNags(NagSet nags, MoveId moveId = CURRENT_MOVE);
    /** Clear the nags from the node @p moveId */
    bool clearNags(MoveId moveId = CURRENT_MOVE);

	// **** tree information methods *****
	/** @return whether the game is currently at the start position */
	bool atLineStart(MoveId moveId = CURRENT_MOVE) const;
	bool atGameStart(MoveId moveId = CURRENT_MOVE) const;
	/** @return whether the game is at the end of the current variation */
	bool atLineEnd(MoveId moveId = CURRENT_MOVE) const;
	bool atGameEnd(MoveId moveId = CURRENT_MOVE) const;
	/** Counts the number of moves, comments and nags, in mainline, to the end of the game */
	void moveCount(int* moves, int* comments, int* nags);
	/** @return number of half moves made since the beginning of the game */
	int ply(MoveId moveId = CURRENT_MOVE) const;
	/** @return current move. Equals to (ply-1)/2+1 for standard games, but may be different
	*/
	int moveNumber(MoveId moveId = CURRENT_MOVE) const;
	/** @return number of ply for the whole game (mainline only) */
	int plyCount() const;
	/** @return number of current variation */
	MoveId variationNumber(MoveId moveId = CURRENT_MOVE) const;
	/** @return number of variations at the current position */
	int variationCount(MoveId moveId = CURRENT_MOVE) const;
	/** @return true if the game has been modified */
	bool isModified() const;
	/** Clear/set game's @p modified flag. */
	void setModified(bool set);
	/** @return moveId of the previous move */
	MoveId previousMove() const;
	/** @return moveId of the next move */
	MoveId nextMove() const;
	/** @return moveId of the parent node */
	MoveId parentMove() const;
	/** @return list of variation at the current move */
	const QList<MoveId>& variations() const;

	// ***** Moving through game *****
	/** Moves to the beginning of the game */
	void moveToStart();
	/** Moves to the end of the game */
	void moveToEnd();
	/** Moves by given ply, returns actual ply reached */
	int moveByPly(int diff);
	/** Moves to the position corresponding to the given move id */
	void moveToId(MoveId moveId);
	/** Moves to the given ply */
	int moveToPly(int ply);
	/** Move forward the given number of moves, returns actual number of moves made */
	int forward(int count = 1);
	/** Move back the given number of moves, returns actual number of moves undone */
	int backward(int count = 1);
    /** Moves forward if the next move matches (from,to) */
    bool findNextMove(Square from, Square to);

	/** Enters the variation that corresponds to moveId. moveId must be a MoveId that
	    corresponds to a subvariation of the current position. Compared to moveToId
            this function runs in constant time.
	*/
	void enterVariation(const MoveId& moveId);

	// ***** game modification methods *****
	/** Adds a move at the current position, returns the move id of the added move */
	MoveId addMove(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet());
	/** Adds a move at the current position, returns the move id of the added move */
	MoveId addMove(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet());
    /** Replace the move after the current position */
    bool replaceMove(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet());
	/** Replace the move after the current position */
	bool replaceMove(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet());
	/** Adds a move at the current position as a variation,
	 * returns the move id of the added move */
	MoveId addVariation(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet());
	/** Adds a move at the current position as a variation,
	 * returns the move id of the added move */
	MoveId addVariation(const MoveList& moveList, const QString& annotation = QString());
	/** Adds a move at the current position as a variation,
	 * returns the move id of the added move */
	MoveId addVariation(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet());
	/** Promotes the given variation to the main line, returns true if successful */
	bool promoteVariation(MoveId variation);
	/** Removes the given variation, returns true if successful */
	bool removeVariation(MoveId variation);
	/** Removes all variations and mainline moves after the current position,
	* or before the current position if @p position == BeforeMove */
	void truncateVariation(Position position = AfterMove);
	/** Removes all tags and moves */
	void clear();
	/** Set the game start position */
	void setStartingBoard(const Board& startingBoard);
	/** Set the game start position from FEN. */
	void setStartingBoard(const QString& fen);
    /** set comment associated with game */
    void setGameComment( const QString& gameComment);

	/* Manipulating and querying tags */
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
	/** Set the game result */
	void setResult(Result result);

	// Searching
	/** Search game to see if given position exists, if it does return move id */
	MoveId findPosition(const Board& position);

	/** @return ECO code for the game */
	QString ecoClassify();

	/** Method that loads a file containing ECO classifications for use by the ecoClassify method. Returns true if successful */
	static bool loadEcoFile(const QString& ecoFile);

	/* Debug */
	/** Dump a move node using qDebug() */
	void dumpMoveNode(MoveId moveId = CURRENT_MOVE);
	/** Repeatedly call dumpMoveNode for all nodes */
	void dumpAllMoveNodes();

private:

    QString specialAnnotation(QString& annotation, QString specialMark ); // [%csl  [%cal

	struct MoveNode {
		Move move;
		NagSet nags;
		MoveId previousNode;
		MoveId nextNode;
		MoveId parentNode;
		bool removed;
		int ply;
		QList <MoveId> variations;
		void remove()	{parentNode = previousNode = nextNode = NO_MOVE; removed = true;}
		MoveNode()		{parentNode = nextNode = previousNode = NO_MOVE; removed = false; ply = 0;}
	};

    /** commment associated with game */
    QString m_gameComment;

	/** List of nodes */
	QList <MoveNode> m_moveNodes;
	/** Keeps the current node in the game */
	MoveId m_currentNode;
	/** Keeps the start position of the game */
	Board m_startingBoard;
	/** Keeps the current position of the game */
	Board m_currentBoard;
	/** Keeps the start ply of the game, 0 for standard starting position */
	int m_startPly;
	/** Flag indicating if the game has been modified */
	bool m_isModified;
	/** Start annotations for each variation */
	QMap <MoveId, QString> m_variationStartAnnotations;
	/** Annotations for move nodes */
	QMap <MoveId, QString> m_annotations;

    /** SquareAnnotations for move nodes */
    QMap <MoveId, QString> m_squareAnnotations;

    /** ArrowAnnotations for move nodes */
    QMap <MoveId, QString> m_arrowAnnotations;

    /** Map keeping pgn tags of the game */
	QMap<QString, QString> m_tags;

    /** Remaining Time Annotations for move nodes */
    QMap <MoveId, QString> m_clkAnnotations;

    /** Elapsed Time Annotations for move nodes */
    QMap <MoveId, QString> m_egtAnnotations;

	// **** memory  management methods ****
	/** Remove all removed nodes */
	void compact();
	/** Removes the node at @p moveId */
	void removeNode(MoveId moveId = CURRENT_MOVE);

	/** Checks if a moveId is valid, returns the moveId if it is, 0 if not */
	MoveId nodeValid(MoveId moveId = CURRENT_MOVE) const;
	/** Change parent of each move of a variation. */
	void reparentVariation(MoveId variation, MoveId parent);

	//eco data
	static QMap<quint64, QString> m_ecoPositions;
};


#endif	// __GAME_H__

