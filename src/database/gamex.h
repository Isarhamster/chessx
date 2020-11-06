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
#include "board.h"
#include "gameid.h"
#include "move.h"
#include "nag.h"
#include "result.h"

#define ROOT_NODE 0
#define NO_MOVE -1
#define CURRENT_MOVE -2

typedef short MoveId;

class SaveRestoreMove;

class GameCursor
{
public:
    #pragma pack(push, 2)
    struct Node
    {
        MoveId previousNode;
        MoveId nextNode;
        MoveId parentNode;
        short m_ply;
        Move move;
        QList<MoveId> variations;
        void remove()
        {
            parentNode = previousNode = nextNode = NO_MOVE;
            variations.clear();
            m_ply |= 0x8000;
        }
        Node()
        {
            parentNode = nextNode = previousNode = NO_MOVE;
            variations.clear();
            m_ply = 0;
        }
        void SetPly(short ply) { Q_ASSERT(m_ply<0x7FFF); m_ply = ply; }
        short Ply() const { return m_ply & 0x7FFF; }
        bool Removed() const { return (m_ply & 0x8000); }
        inline bool operator==(const struct Node& c) const
        {
            return (move == c.move &&
                    variations == c.variations &&
                    m_ply == c.m_ply);
        }
    };
    #pragma pack(pop)

    GameCursor();
    GameCursor(const GameCursor& rhs);
    GameCursor& operator=(const GameCursor& rhs);
    // TODO: maybe implement moving constructor/assignment later
    GameCursor(GameCursor&& rhs) = delete;
    GameCursor& operator=(GameCursor&& rhs) = delete;
    ~GameCursor();

    /** @returns initial position */
    const BoardX& initialBoard() const { return m_startingBoard; }

    /** @return current position */
    const BoardX* currentBoard() const { return m_currentBoard; }
    BoardX* currentBoard() { return m_currentBoard; }

    void unmountBoard();

    /** Re-initialize all fields  */
    void clear();
    void clear(const QString& fen, bool chess960 = false);

    /** Checks if a \p moveId is valid
     *  @returns
     *  - on failure: \p NO_MOVE
     *  - on success: index suitable for subscripting \p m_nodes
     */
    MoveId makeNodeIndex(MoveId moveId = CURRENT_MOVE) const;

    /** @return the move at node @p moveId. */
    Move move(MoveId moveId = CURRENT_MOVE) const;
    Move& moveAt(MoveId moveId) { return m_nodes[moveId].move; }
    /** @return current move id. */
    MoveId currMove() const { return m_currentNode; }
    /** @return moveId of the previous move */
    MoveId prevMove() const { return m_nodes[m_currentNode].previousNode; }
    MoveId prevMove(MoveId moveId) const { return m_nodes[moveId].previousNode; }
    /** @return moveId of the next move */
    MoveId nextMove() const { return m_nodes[m_currentNode].nextNode; }
    MoveId nextMove(MoveId moveId) const { return m_nodes[moveId].nextNode; }
    /** @return moveId of the parent node */
    MoveId parentMove() const { return m_nodes[m_currentNode].parentNode; }
    MoveId parentMove(MoveId moveId) const { return m_nodes[moveId].parentNode; }
    /** @return number of variations at the current position */
    int variationCount(MoveId moveId = CURRENT_MOVE) const;
    /** @return list of variation at the current move */
    const QList<MoveId>& variations() const;
    const QList<MoveId>& variations(MoveId moveId) const { return m_nodes[moveId].variations; }
    /** @returns amount of allocated nodes */
    int capacity() const { return m_nodes.size(); }

    /** @return whether the current position is in the mainline */
    bool isMainline(MoveId moveId = CURRENT_MOVE) const;
    /** @return moveId of the top main line */
    MoveId mainLineMove() const;

    /** @return whether the game is at the start of the current variation */
    bool atLineStart(MoveId moveId = CURRENT_MOVE) const;
    /** @return whether the game is at the end of the current variation */
    bool atLineEnd(MoveId moveId = CURRENT_MOVE) const;

    /** @return whether the game is currently at the start position */
    bool atGameStart(MoveId moveId = CURRENT_MOVE) const;
    /** @return whether the game is currently at the end of main variation */
    bool atGameEnd(MoveId moveId = CURRENT_MOVE) const;

    /** @return number of ply for the whole game (mainline only) */
    int plyCount() const;
    /** @return number of half moves made since the beginning of the game */
    int plyNumber(MoveId moveId = CURRENT_MOVE) const;
    /** @return current move. Equals to (ply-1)/2+1 for standard games, but may be different */
    int moveNumber(MoveId moveId = CURRENT_MOVE) const;
    /** @return number of move nodes in the main line */
    int countMoves() const;

    /** Get the first move of a variation */
    MoveId variationStartMove(MoveId variation = CURRENT_MOVE) const;
    /** @return number of current variation */
    MoveId variationNumber(MoveId moveId = CURRENT_MOVE) const;
    /** @return true if the referenced variation has siblings */
    bool variationHasSiblings(MoveId variation = CURRENT_MOVE) const;

    /** Moves to the position corresponding to the given move id */
    bool moveToId(MoveId moveId, QString* algebraicMoveList=nullptr);
    /** Move forward the given number of moves, returns actual number of moves made */
    int forward(int count = 1);
    /** Move back the given number of moves, returns actual number of moves undone */
    int backward(int count = 1);
    /** Move to the end of current variation */
    bool moveToLineEnd();
    /** Moves to the beginning of the game */
    bool moveToStart();
    /** Moves to the end of the game */
    bool moveToEnd();
    /** Enters the variation that corresponds to \p moveId.
        \p moveId must correspond to a subvariation of the current position.
        Compared to \p moveToId() this function runs in constant time.
    */
    void moveIntoVariation(MoveId moveId);

    /** Adds a move at the current position.
        @returns the move id of the added move
     */
    MoveId addMove(const Move& move);
    /** Adds a move at the current position as a variation.
        @returns the move id of the added move
     */
    MoveId addVariation(const Move& move);

    /** Mark move subtree for removal */
    void remove(MoveId moveId, QList<MoveId>* removed = nullptr);
    /** Mark nodes after \p moveId (variations and continuation) for removal, excluding the node itself */
    void truncateFrom(MoveId moveId, QList<MoveId>* removed = nullptr);
    /** Mark nodes preceding \p moveId  for removal */
    void truncateUpto(MoveId moveId, QList<MoveId>* removed = nullptr);
    /** Promotes the given variation to the main line */
    void promoteVariation(MoveId variation);
    /** Decide if moveVariationUp() can be executed */
    bool canMoveVariationUp(MoveId moveId) const;
    /** Decide if moveVariationDown() can be executed */
    bool canMoveVariationDown(MoveId moveId) const;
    /** Move the variation @p moveId of the curent node up in the list of variations */
    bool moveVariationUp(MoveId moveId);
    /** Move the variation @p moveId of the curent node down in the list of variations */
    bool moveVariationDown(MoveId moveId);
    /** Removes the given variation, returns true if successful */
    bool removeVariation(MoveId variation);
    /** Remove all variations */
    void removeVariations();
    /** Remove nodes marked for removal */
    QMap<MoveId, MoveId> compact();

    /** Change parent of each move of a variation. */
    void reparentVariation(MoveId variation, MoveId parent);

    /** Search game to see if given position exists and returns the move id, otherwise NO_MOVE */
    MoveId findPosition(const BoardX& position) const;

    /** Dump a move node using qDebug() */
    void dumpMoveNode(MoveId moveId = CURRENT_MOVE) const;

    /** compare game moves and annotations */
    int isEqual(const GameCursor& rhs) const { return m_nodes == rhs.m_nodes; }

private:
    /** Keeps the current position of the game */
    BoardX* m_currentBoard;
    /** List of nodes */
    QList<Node> m_nodes;
    /** Keeps the current node in the game */
    MoveId m_currentNode;
    /** Keeps the start ply of the game, 0 for standard starting position */
    short m_startPly;
    /** Keeps the start position of the game */
    BoardX m_startingBoard;

    void initCursor();
};

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
        FilterNone = 0x00,
        FilterTan  = 0x01,
        FilterCan  = 0x02,
        FilterEval = 0x04,
        FilterAll  = (FilterTan | FilterCan | FilterEval),
    };

    static const QStringList s_specList;

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
    QString textAnnotation(MoveId moveId = CURRENT_MOVE, Position position = AfterMove, AnnotationFilter f = FilterNone) const;
    /** Show annotations on the board */
    void indicateAnnotationsOnBoard();
    /** @return squareAnnotation at move at node @p moveId. */
    QString squareAnnotation(MoveId moveId = CURRENT_MOVE) const;
    /** @return arrowAnnotation at move at node @p moveId. */
    QString arrowAnnotation(MoveId moveId = CURRENT_MOVE) const;
    /** @return annotation at move at node @p moveId. */
    QString specAnnotation(const QRegExp &r, MoveId moveId = CURRENT_MOVE) const;
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
    /** Edits the comment associated with move at node @p moveId */
    bool editAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE, Position position = AfterMove);
    /** Append to existing annotations associated with move at node @p moveId */
    bool appendAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE, Position position = AfterMove);
    /** Sets the squareAnnotation associated with move at node @p moveId */
    bool setSquareAnnotation(QString squareAnnotation);

    /** Append a square to the existing lists of square annotations, if there is none, create one */
    bool appendSquareAnnotation(chessx::Square s, QChar colorCode);

    /** Append an arrow to the existing lists of arrows, if there is none, create one */
    bool appendArrowAnnotation(chessx::Square dest, chessx::Square src, QChar colorCode);

    /** Sets the arrowAnnotation associated with current move */
    bool setArrowAnnotation(QString arrowAnnotation);

    /** Get a string with all special annotations including square brackets etc. */
    QString specAnnotations(MoveId moveId = CURRENT_MOVE, Position position = AfterMove) const;

    /** Get a the current annotation for an evaluation string, d is modified only if there is such an evaluation */
    void evaluation(double& d) const;

    /** Adds a nag to move at node @p moveId */
    bool dbAddNag(Nag nag, MoveId moveId = CURRENT_MOVE);
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
    bool dbMoveToId(MoveId moveId, QString* algebraicMoveList=nullptr);
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
    bool promoteVariation(MoveId variation);
    /** Removes the given variation, returns true if successful */
    bool removeVariation(MoveId variation);
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

protected:
    /** Find the point in the this game where @p otherGame fits in the next time.
        @retval Node from where the merging shall start in other game */
    MoveId findMergePoint(const GameX &otherGame);
    /** Find the next illegal position in all variations and mainline moves after the current position, and cut the game from there */
    void truncateVariationAfterNextIllegalPosition();
    /** Removes the node at @p moveId */
    void removeNode(MoveId moveId = CURRENT_MOVE);

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

    void removeTimeCommentsFromMap(AnnotationMap& map);

    friend class SaveRestoreMove;
};

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

