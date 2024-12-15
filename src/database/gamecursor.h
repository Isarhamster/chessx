#ifndef GAMECURSOR_H
#define GAMECURSOR_H

#include <QObject>
#include "board.h"
#include "move.h"

#define ROOT_NODE 0
#define NO_MOVE -1
#define CURRENT_MOVE -2
#define PREV_MOVE -3
#define NEXT_MOVE -4
#define PARENT_MOVE -5

typedef int MoveId;

class GameCursor
{
public:
    struct Node
    {
        MoveId previousNode; /* points to the previous node in a line, in case of a line start, it also points to the parent node */
        MoveId nextNode; /* points to the next node in a line */
        MoveId parentNode; /* points to the parent node when inside a line (all nodes in the line have this!) */
        short m_ply;
        Move move;
        QList<MoveId> variations;
        void remove()
        {
            parentNode = previousNode = nextNode = NO_MOVE;
            variations.clear();
            setRemoved();
        }
        void setRemoved()
        {
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
    int nextMoveNumber(bool atStartOfLine, bool atEndOfLine) const;
    /** @return number of move nodes in the main line */
    int countMoves() const;

    /** Get the first move of a variation */
    MoveId variationStartMove(MoveId variation = CURRENT_MOVE) const;
    /** @return number of current variation */
    MoveId variationNumber(MoveId moveId = CURRENT_MOVE) const;
    /** @return true if the referenced variation has siblings */
    bool variationHasSiblings(MoveId variation = CURRENT_MOVE) const;
    /** @return Find the next Node back where the variation branches off  */
    MoveId variationBranchPoint(MoveId variation = CURRENT_MOVE) const;

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
    void clearDummyNodes();
    QMap<MoveId, MoveId> compact();
    void removeNullLines();
    bool isRemoved(MoveId moveId) const;
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
    /** Keeps the start position of the game */
    BoardX m_startingBoard;

    void initCursor();
};

#endif // GAMECURSOR_H
