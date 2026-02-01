/***************************************************************************
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <QObject>
#include <QRegularExpression>
#include "annotation.h"
#include "board.h"
#include "gamecursor.h"
#include "move.h"
#include "nag.h"
#include "result.h"

class SaveRestoreMove;

/** @ingroup Core

   The GameX class represents a chess game. This is a complete rewrite, with simpler
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

typedef QHash<QString, QString> TagMap;
typedef QHashIterator<QString, QString> TagMapIterator;

class GameX : public QObject
{
    Q_OBJECT

public :
    /**
        Flags indicating how a move string should be constructed

        These flags may be or-ed together to specify what should be included in
        a move string.
    */
    enum MoveStringFlags
    {
        MoveOnly = 0,      /**< Only the algebraic notation should be included */
        WhiteNumbers = 1,  /**< White moves should be preceded by a move number */
        BlackNumbers = 2,  /**< Black moves should be preceded by a move number */
        Nags = 4,          /**< Nags/symbolic annotation should be included */
        FullDetail = 7,    /**< Specifies all move numbers and nags should be included */
        TranslatePiece = 8 /**< Translate using the user-defined translation */
    };
    enum Position
    {
        BeforeMove,
        AfterMove
    };
    enum NextPreviousMove
    {
        NextMove,
        PreviousMove
    };
    enum AnnotationFilter
    {
        FilterNone    = 0x00,
        FilterTan     = 0x01,
        FilterCan     = 0x02,
        FilterTanCan  = (FilterTan | FilterCan),
        FilterEval    = 0x04,
        FilterTanEval = (FilterTan | FilterEval),
        FilterCanEval = (FilterCan | FilterEval),
        FilterAll  = (FilterTan | FilterCan | FilterEval),
    };

    static const QList<QRegularExpression> s_specList;

    GameX();
    GameX(const GameX& game);
    GameX& operator=(const GameX& game);
    virtual ~GameX();

    void unmountBoard() { m_moves.unmountBoard(); }

    const GameCursor& cursor() const { return m_moves; }
    GameCursor& cursor() { return m_moves; }

    // **** Querying game information ****
    /** compare game moves and annotations */
    int isEqual(const GameX& game) const;
    /** compare game moves and annotations */
    int isBetterOrEqual(const GameX& game) const;
    /** @return current position */
    const BoardX& board() const;
    /** @return current position in FEN */
    QString toFen() const;
    /** @return current position in human readable FEN */
    QString toHumanFen() const;
    /** @return start position of game */
    BoardX startingBoard() const { return m_moves.initialBoard(); }
    /** @return game result */
    Result result() const;
    /** @return comment at move at node @p moveId including visual hints for diagrams. */
    QString annotation(MoveId moveId = CURRENT_MOVE, Position position = AfterMove) const;
    /** @return comment at move at node @p moveId. */
    static QString cleanAnnotation(QString s, AnnotationFilter f);
    QString textAnnotation(MoveId moveId = CURRENT_MOVE, Position position = AfterMove, AnnotationFilter f = FilterNone) const;
    AnnotationFilter textFilter() const;
    AnnotationFilter textFilter2() const;
    /** Show annotations on the board */
    void indicateAnnotationsOnBoard();
    /** @return squareAnnotation at move at node @p moveId. */
    QString squareAnnotation(MoveId moveId = CURRENT_MOVE) const;
    /** @return arrowAnnotation at move at node @p moveId. */
    QString arrowAnnotation(MoveId moveId = CURRENT_MOVE) const;
    /** @return annotation at move at node @p moveId. */
    QString specAnnotation(const QRegularExpression &r, MoveId moveId = CURRENT_MOVE, QString *found=0) const;
    /** @return time annotation (either egt or clock) at move at node @p moveId. */
    QString timeAnnotation(MoveId moveId = CURRENT_MOVE, Position position = AfterMove) const;
    void setTimeAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE);
    /** @return @p true if a move can have annotation before the move. */
    bool canHaveStartAnnotation(MoveId moveId = CURRENT_MOVE) const;
    /** @return nags for move at node @p moveId */
    NagSet nags(MoveId moveId = CURRENT_MOVE) const;
    /** @return next move in short algebraic notation, returns empty string if no such move */
    QString moveToSan(MoveStringFlags flags = MoveOnly,
                      NextPreviousMove nextPrevious = NextMove, MoveId moveId = CURRENT_MOVE,
                      QString* annotations = nullptr, NagSet* nagSet = nullptr);
    /** Query event info and date information for display in game browser etc. */
    QString eventInfo() const;

    // **** node modification methods ****
    /** Sets the comment associated with move at node @p moveId */
    bool dbSetAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE, Position position = AfterMove);
    bool dbAppendAnnotation(QString a, MoveId moveId = CURRENT_MOVE, Position position = AfterMove);
    bool dbPrependAnnotation(QString a, char delimiter=' ', MoveId moveId = CURRENT_MOVE, Position position = AfterMove);
    /** Sets the comment associated with move at node @p moveId */
    bool setAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE, Position position = AfterMove);
    bool prependAnnotation(QString a, char delimiter=' ', MoveId moveId = CURRENT_MOVE, Position position = AfterMove);
    /** Edits the comment associated with move at node @p moveId */
    bool editAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE, Position position = AfterMove);
    /** Append to existing annotations associated with move at node @p moveId */
    bool appendAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE, Position position = AfterMove);

    /** Append a square to the existing lists of square annotations, if there is none, create one */
    bool appendSquareAnnotation(chessx::Square s, QChar colorCode);

    /** Append an arrow to the existing lists of arrows, if there is none, create one */
    bool appendArrowAnnotation(chessx::Square dest, chessx::Square src, QChar colorCode);

    /** Sets a special annotation associated with current move */
    bool setSpecAnnotation(const Annotation& a);

    /** Get a string with all special annotations including square brackets etc. */
    QString specAnnotations(MoveId moveId = CURRENT_MOVE, Position position = AfterMove) const;
    QString specAnnotations(QString s) const;

    /** Get a the current annotation for an evaluation string, d is modified only if there is such an evaluation */
    void evaluation(double& d) const;

    /** Adds a nag to move at node @p moveId */
    void dbAddNag(Nag nag, MoveId moveId = CURRENT_MOVE);
    bool addNag(Nag nag, MoveId moveId = CURRENT_MOVE);
    /** Sets nags for move at node @p moveId */
    bool setNags(NagSet nags, MoveId moveId = CURRENT_MOVE);
    /** Clear the nags from the node @p moveId */
    bool clearNags(MoveId moveId = CURRENT_MOVE);

    // **** tree information methods *****
    Move move(MoveId moveId = CURRENT_MOVE) const { return m_moves.move(moveId); }

    MoveId currentMove() const { return m_moves.currMove(); }
    MoveId previousMove() const { return m_moves.prevMove(); }
    MoveId nextMove() const { return m_moves.nextMove(); }
    MoveId parentMove() const { return m_moves.parentMove(); }

    int variationCount(MoveId moveId = CURRENT_MOVE) const { return m_moves.variationCount(moveId); }
    const QList<MoveId>& variations() const { return m_moves.variations(); }

    bool isMainline(MoveId moveId = CURRENT_MOVE) const { return m_moves.isMainline(moveId); }
    bool atLineStart(MoveId moveId = CURRENT_MOVE) const { return m_moves.atLineStart(moveId); }
    bool atLineEnd(MoveId moveId = CURRENT_MOVE) const { return m_moves.atLineEnd(moveId); }
    bool atGameStart(MoveId moveId = CURRENT_MOVE) const { return m_moves.atGameStart(moveId); }
    bool atGameEnd(MoveId moveId = CURRENT_MOVE) const { return m_moves.atGameEnd(moveId); }

    int plyCount() const { return m_moves.plyCount(); }
    int ply(MoveId moveId = CURRENT_MOVE) const { return m_moves.plyNumber(moveId); }
    int moveNumber(MoveId moveId = CURRENT_MOVE) const { return m_moves.moveNumber(moveId); }

    MoveId variationStartMove(MoveId variation = CURRENT_MOVE) const { return m_moves.variationStartMove(variation); }
    MoveId variationNumber(MoveId moveId = CURRENT_MOVE) const { return m_moves.variationNumber(moveId); }
    bool variationHasSiblings(MoveId variation = CURRENT_MOVE) const { return m_moves.variationHasSiblings(variation); }

    /** Counts the number of moves, comments and nags, in mainline, to the end of the game */
    void moveCount(int* moves, int* comments, int* nags=nullptr) const;
    /** Determine if game contains something reasonable */
    bool isEmpty() const;

    // ***** Moving through game *****
    bool dbMoveToId(MoveId moveId, QString* algebraicMoveList=nullptr) { return m_moves.moveToId(moveId, algebraicMoveList); }
    int forward(int count = 1);
    int backward(int count = 1);
    void moveToStart();
    void moveToEnd();
    void moveToLineEnd();

    /** Moves by given ply, returns actual ply reached */
    int moveByPly(int diff);
    void moveToId(MoveId moveId);
    /** Moves forward if the next move matches (from,to,promotionPiece) */
    bool findNextMove(chessx::Square from, chessx::Square to, PieceType promotionPiece = None);
    bool findNextMove(Move m);
    bool hasNextMove() const;

    /** Enters the variation that corresponds to moveId. moveId must be a MoveId that
        corresponds to a subvariation of the current position. Compared to moveToId
            this function runs in constant time.
    */
    void enterVariation(const MoveId& moveId);

    // ***** game modification methods *****
    void clearDummyNodes();
    /** Adds a move at the current position, returns the move id of the added move */
    MoveId dbAddMove(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet());
    /** Adds a move at the current position, returns the move id of the added move */
    MoveId addMove(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet());
    /** Adds a move at the current position, returns the move id of the added move */
    MoveId dbAddSanMove(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet());
    /** Adds a move at the current position, returns the move id of the added move */
    MoveId addMove(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet());
    /** Adds a move at the current position, returns the move id of the added move */
    MoveId addMoveFrom64Char(const QString& qcharboard);
    /** Replace the move after the current position */
    bool replaceMove(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet(), bool bReplace = true);
    /** Replace the move after the current position */
    bool replaceMove(const QString& sanMove);
    /** Insert the move after the current position */
    bool insertMove(Move m);
    /** Append a line at the current position,
     * returns the move id of the added move */
    MoveId addLine(const Move::List& moveList, const QString& annotation = QString());
    /** Adds a move at the current position as a variation,
     * returns the move id of the added move */
    MoveId addVariation(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet());
    /** Adds a move at the current position as a variation,
     * returns the move id of the added move */
    MoveId addVariation(const Move::List& moveList, const QString& annotation = QString());
    /** Adds a move at the current position as a variation,
     * returns the move id of the added move */
    MoveId addVariation(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet());
    /** Append a line at the current position,
     * returns the move id of the added move */
    MoveId dbAddLine(const Move::List& moveList, const QString& annotation);
    /** Adds a move at the current position as a variation,
     * returns the move id of the added move */
    MoveId dbAddVariation(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet());
    /** Adds a move at the current position as a variation,
     * returns the move id of the added move */
    MoveId dbAddVariation(const Move::List& moveList, const QString& annotation = QString());
    /** Adds a move at the current position as a variation,
     * returns the move id of the added move */
    MoveId dbAddSanVariation(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet());
    /** Merge current node of @p otherGame into this game */
    bool mergeNode(GameX &otherGame);
    /** Merge @p otherGame starting from otherGames current position into this game as a new mainline */
    bool mergeAsMainline(GameX &otherGame);
    /** Merge @p otherGame starting from otherGames current position into this game as a new variation */
    bool mergeAsVariation(GameX &otherGame);
    /** Merge @p otherGame variations starting from otherGames current position into this game as new variations */
    bool mergeVariations(GameX &otherGame);

    /** Merge GameX @p g into this game */
    void mergeWithGame(const GameX& g);
    /** Merge GameX @p g into this game */
    void dbMergeWithGame(const GameX& g);
    /** Promotes the given variation to the main line, returns true if successful */
    void promoteVariation(MoveId variation);
    void dbPromoteVariation(MoveId variation);
    /** Removes the given variation, returns true if successful */
    bool removeVariation(MoveId variation);
    bool dbRemoveVariation(MoveId variation);
    /** Removes all variations and mainline moves after the current position,
    * or before the current position if @p position == BeforeMove */
    void dbTruncateVariation(Position position = AfterMove);
    void truncateVariation(Position position = AfterMove);
    /** Removes all tags and moves */
    void clear();
    /** Set the game start position from FEN. */
    void dbSetStartingBoard(const QString& fen, bool chess960 = false);
    /** set comment associated with game */
    void setGameComment(const QString& gameComment);
    /** Remove all variations */
    void removeVariations();
    void removeVariationsDb();
    void removeNullLines();
    void removeNullLinesDb();
    /** Remove all Comments */
    void removeComments();
    /** Remove all time Comments */
    void removeTimeComments();
    /** Remove all Comments w/o noticiations */
    void removeCommentsDb();
    /** Remove all time Comments w/o noticiations */
    void removeTimeCommentsDb();
    /* Manipulating and querying tags */
    /** Removes all tags */
    void clearTags();
    /** @return value of the given tag */
    QString tag(const QString& tag) const;
    /** @return a map of all tags in the game */
    const TagMap &tags() const;
    /** Sets the value of the given tag */
    void setTag(const QString& tag, const QString& value);
    /** Sets the value of the configured source tag */
    void setSourceTag(const QString& value);
    /** Query existance of tag */
    bool hasTag(const QString& tag) const;
    /** Removes a tag */
    void removeTag(const QString& tag);
    /** Set the game result */
    void dbSetResult(Result result);
    void setResult(Result result);

    void dbSetChess960(bool b);
    bool isChess960() const;

    // Searching
    /** @return true if the move @p from @p to is already main move or variation */
    bool currentNodeHasMove(chessx::Square from, chessx::Square to) const;
    /** @return true if the move @p from @p to is already in a variation */
    bool currentNodeHasVariation(chessx::Square from, chessx::Square to) const;
    /** Return the list of variations of the current node */
    const QList<MoveId>& currentVariations() const;

    /** Evaluate a list of scores for the complete game (mainline only) */
    void scoreMaterial(QList<double> &scores) const;
    void scoreEvaluations(QList<double> &evaluations) const;

    /** @return ECO code for the game */
    QString ecoClassify() const;
    /** @return true if current pos is in the ECO list */
    bool isEcoPosition() const;

    /* Debug */
    /** Dump annotatios for move @p moveId using qDebug() */
    void dumpAnnotations(MoveId moveId) const;
    /** Repeatedly call dumpMoveNode for all nodes */
    void dumpAllMoveNodes() const;
    /** Start all variations with an initial character comment */
    void enumerateVariations(MoveId moveId, char a);
    /** Move the variation @p moveId of the curent node up in the list of variations */
    void moveVariationUp(MoveId moveId);
    /** Move the variation @p moveId of the curent node down in the list of variations */
    void moveVariationDown(MoveId moveId);
    /** Copy in a game and set it as modified (different from operator=) */
    void copyFromGame(const GameX& g);

    int resultAsInt() const;
    void setStartingBoard(const BoardX &startingBoard, QString text, bool chess960 = false);

    MoveId lastMove() const;

    bool positionRepetition3(const BoardX &board) const;
    bool insufficientMaterial(const BoardX &b) const;

    void setNeedsCleanup(bool value);

    bool needsCleanup() const;
    /** Removes the node at @p moveId */
    void removeNode(MoveId moveId = CURRENT_MOVE);

protected:
    /** Find the point in the this game where @p otherGame fits in the next time.
        @retval Node from where the merging shall start in other game */
    MoveId findMergePoint(const GameX &otherGame);
    /** Find the next illegal position in all variations and mainline moves after the current position, and cut the game from there */
    void truncateVariationAfterNextIllegalPosition();


    void dbIndicateAnnotationsOnBoard();
    bool positionRepetition(const BoardX &board);
signals:
    void signalGameModified(bool,GameX,QString);
    void signalMoveChanged();

private:
    GameCursor m_moves;

    using MoveNode = GameCursor::Node;

    typedef QMap<MoveId, QString> AnnotationMap;

    /** Start annotations for each variation */
    AnnotationMap m_variationStartAnnotations;
    /** Annotations for move nodes */
    AnnotationMap m_annotations;
    /** NAGs for move nodes */
    QMap<MoveId, NagSet> m_nags;

    /** Map keeping pgn tags of the game */
    TagMap m_tags;

    // **** memory  management methods ****
    /** Remove all removed nodes */
    void compact();
    bool m_needsCleanup;

    void removeTimeCommentsFromMap(AnnotationMap& map);

    friend class SaveRestoreMove;
};

inline GameX::AnnotationFilter operator|(GameX::AnnotationFilter a, GameX::AnnotationFilter b)
{
    return static_cast<GameX::AnnotationFilter>(static_cast<int>(a) | static_cast<int>(b));
}

class SaveRestoreMove
{
public:
    explicit SaveRestoreMove(GameX& game)
    {
        m_saveGame = &game;
        m_saveMoveValue = game.currentMove();
    }
    ~SaveRestoreMove()
    {
        m_saveGame->dbMoveToId(m_saveMoveValue);
    }
private:
    GameX* m_saveGame;
    MoveId m_saveMoveValue;
};

#endif	// GAME_H_INCLUDED

