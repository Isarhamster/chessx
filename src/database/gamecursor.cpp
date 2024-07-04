/***************************************************************************
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QtDebug>
#include <QFile>
#include "annotation.h"
#include "gamecursor.h"
#include "settings.h"
#include "tags.h"

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

GameCursor::GameCursor()
    : m_currentBoard(new BoardX)
    , m_nodes()
    , m_currentNode(0)
    , m_startPly(0)
    , m_startingBoard()
{
    m_startingBoard.setStandardPosition();
    initCursor();
}

GameCursor::GameCursor(const GameCursor& rhs)
    : m_currentBoard(new BoardX)
    , m_nodes(rhs.m_nodes)
    , m_currentNode(rhs.m_currentNode)
    , m_startPly(rhs.m_startPly)
    , m_startingBoard(rhs.m_startingBoard)
{
    if (rhs.m_currentBoard)
    {
        *m_currentBoard = *rhs.m_currentBoard;
    }
}

GameCursor& GameCursor::operator=(const GameCursor& rhs)
{
    if (this != &rhs)
    {
        m_nodes = rhs.m_nodes;
        m_currentNode = rhs.m_currentNode;
        m_startPly = rhs.m_startPly;
        m_startingBoard = rhs.m_startingBoard;
        if (m_currentBoard && rhs.m_currentBoard)
        {
            *m_currentBoard = *rhs.m_currentBoard;
        }
    }
    return *this;
}

GameCursor::~GameCursor()
{
    unmountBoard();
}

void GameCursor::initCursor()
{
    m_nodes.append(Node());
    if (m_currentBoard)
    {
        m_currentNode = ROOT_NODE;
        *m_currentBoard = m_startingBoard;
    }
    else
    {
        m_currentNode = NO_MOVE;
    }
}

void GameCursor::unmountBoard()
{
    if (m_currentBoard)
    {
        delete m_currentBoard;
        m_currentBoard = nullptr;
    }
}

void GameCursor::clear()
{
    m_nodes.clear();
    m_startingBoard.setStandardPosition();
    m_startPly = 0;
    initCursor();
}

void GameCursor::clear(const QString& fen, bool chess960)
{
    m_nodes.clear();
    m_startingBoard.setChess960(chess960);
    m_startingBoard.fromFen(fen);
    m_startPly = (m_startingBoard.moveNumber() - 1) * 2 + (m_startingBoard.toMove() == Black);
    initCursor();
}

MoveId GameCursor::makeNodeIndex(MoveId moveId) const
{
    if (moveId == NO_MOVE)
    {
        return NO_MOVE;
    }
    if (moveId == CURRENT_MOVE)
    {
        moveId = m_currentNode;
    }
    else if (moveId == PREV_MOVE)
    {
        moveId = m_nodes[m_currentNode].previousNode;
    }
    else if (moveId == NEXT_MOVE)
    {
        moveId = m_nodes[m_currentNode].nextNode;
    }
    bool rangeOk = 0 <= moveId && moveId < m_nodes.size();
    if (!rangeOk || m_nodes[moveId].Removed())
    {
        return NO_MOVE;
    }
    return moveId;
}

Move GameCursor::move(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if(node != NO_MOVE)
    {
        return m_nodes[node].move;
    }
    return Move();
}

int GameCursor::variationCount(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if(node != NO_MOVE)
    {
        return m_nodes[node].variations.size();
    }
    return 0;
}

const QList<MoveId>& GameCursor::variations() const
{
    return m_nodes[m_currentNode].variations;
}

bool GameCursor::isMainline(MoveId moveId) const
{
    if(moveId == 0)
    {
        return true;
    }
    MoveId node = makeNodeIndex(moveId);
    if(node == NO_MOVE)
    {
        return false;
    }
    else
    {
        return m_nodes[node].parentNode == NO_MOVE;
    }
}

MoveId GameCursor::mainLineMove() const
{
    MoveId node = makeNodeIndex(m_currentNode);
    if(node != NO_MOVE)
    {
        bool dive = false;
        while(m_nodes[node].parentNode != NO_MOVE)
        {
            dive = true;
            node = m_nodes[node].parentNode;
        }
        if(dive)
        {
            node = m_nodes[node].nextNode;
        }
    }
    return node;
}

bool GameCursor::atLineStart(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if(node == NO_MOVE)
    {
        return false;
    }
    return (m_nodes[node].previousNode == m_nodes[node].parentNode)
        || m_nodes[node].previousNode == 0;
}

bool GameCursor::atLineEnd(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if (node == NO_MOVE)
    {
        return false;
    }
    return m_nodes[node].nextNode == NO_MOVE;
}

bool GameCursor::atGameStart(MoveId moveId) const
{
    return (makeNodeIndex(moveId) == ROOT_NODE);
}

bool GameCursor::atGameEnd(MoveId moveId) const
{
    return (atLineEnd(moveId) && isMainline(moveId));
}

int GameCursor::plyCount() const
{
    int count = 0;
    MoveId node = ROOT_NODE;

    while(node != NO_MOVE)
    {
        ++count;
        node = m_nodes[node].nextNode;
    }
    // Counted one too much, because we have to start at zero
    // (an empty game has no node 1)
    return count - 1;
}

int GameCursor::plyNumber(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if(node != NO_MOVE)
    {
        return m_nodes[node].Ply();
    }
    return 0;
}

int GameCursor::moveNumber(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if(node != NO_MOVE)
    {
        if(int plyNum = plyNumber(node))
        {
            return (m_startPly + plyNum - 1) / 2 + 1;
        }
        return 0;
    }
    return -1;
}

int GameCursor::countMoves() const
{
    int count = 0;
    MoveId node = 1;
    while((node = makeNodeIndex(node)) != NO_MOVE)
    {
        count += 1;
        node = m_nodes[node].nextNode;
    }
    return count;
}

MoveId GameCursor::variationStartMove(MoveId variation) const
{
    variation = makeNodeIndex(variation);
    if(variation == NO_MOVE)
    {
        return NO_MOVE;
    }
    if(isMainline(variation))
    {
        return NO_MOVE;
    }
    while(!atLineStart(variation))
    {
        variation = m_nodes[variation].previousNode;
    }
    return variation;
}

MoveId GameCursor::variationNumber(MoveId moveId) const
{
    if(isMainline())
    {
        return 0;
    }
    MoveId node = makeNodeIndex(moveId);
    if(node != NO_MOVE)
    {
        MoveId parentNode = m_nodes[node].parentNode;

        while(m_nodes[node].previousNode != parentNode)
        {
            node = m_nodes[node].previousNode;
        }
    }
    return node;
}

MoveId GameCursor::variationBranchPoint(MoveId variation) const
{
    variation = makeNodeIndex(variation);
    if(variation == NO_MOVE)
    {
        return NO_MOVE;
    }

    MoveId branch = m_nodes[variation].parentNode;
    if (branch == NO_MOVE)
    {
        branch = m_nodes[variation].previousNode; // Iterate back to last branchpoint is also thinkable, but probably confusing
        branch = makeNodeIndex(branch);
    }
    else
    {
        MoveId prevNode = variation;
        while ((prevNode = m_nodes[prevNode].previousNode) != NO_MOVE)
        {
            if (m_nodes[prevNode].variations.size())
            {
                branch = prevNode;
                break;
            }
        }
    }

    return (variationCount(branch)) ? branch : NO_MOVE;
}

bool GameCursor::variationHasSiblings(MoveId variation) const
{
    variation = makeNodeIndex(variation);
    return (variationBranchPoint(variation) != NO_MOVE);
}

bool GameCursor::moveToId(MoveId moveId, QString* algebraicMoveList)
{
    moveId = makeNodeIndex(moveId);
    if(moveId == NO_MOVE)
    {
        return false;
    }

    if (m_currentNode != moveId)
    {
        //jump to node, travelling back to start adding the moves to the stack
        MoveId node = moveId;
        QStack<Move> moveStack;
        while(node)
        {
            moveStack.push(m_nodes[node].move);
            node = m_nodes[node].previousNode;
        }

        //reset the board, then make the moves on the stack to create the correct position
        m_currentNode = moveId;
        *m_currentBoard = m_startingBoard;
        while(!moveStack.isEmpty())
        {
            Move m = moveStack.pop();
            m_currentBoard->doMove(m);
            if (algebraicMoveList)
            {
                if (m.isNullMove())
                {
                    // Avoid trouble with a null move - UCI does not specify this and Stockfish makes nonsense
                    algebraicMoveList->clear();
                    algebraicMoveList = nullptr;
                }
                else
                {
                    algebraicMoveList->push_back(m.toAlgebraic());
                    algebraicMoveList->push_back(" ");
                }
            }
        }
    }

    return true;
}

int GameCursor::forward(int count)
{
    int moved = 0;
    while ((m_nodes[m_currentNode].nextNode != NO_MOVE) && (moved < count))
    {
        m_currentNode = m_nodes[m_currentNode].nextNode;
        ++moved;

        m_currentBoard->doMove(m_nodes[m_currentNode].move);
    }
    return moved;
}

int GameCursor::backward(int count)
{
    int moved = 0;
    while((m_nodes[m_currentNode].previousNode >= 0) && (moved < count))
    {
        m_currentBoard->undoMove(m_nodes[m_currentNode].move);
        m_currentNode = m_nodes[m_currentNode].previousNode;
        ++moved;
    }
    return moved;
}

bool GameCursor::moveToLineEnd()
{
    return forward(999) != 0;
}

bool GameCursor::moveToStart()
{
    m_currentNode = 0;
    *m_currentBoard = m_startingBoard;
    return true;
}

bool GameCursor::moveToEnd()
{
    // Move out of variations to mainline
    while(m_nodes[m_currentNode].parentNode != NO_MOVE)
    {
        moveToId(m_nodes[m_currentNode].parentNode);
    }
    // Now move forward to the end of the game
    return moveToLineEnd();
}

void GameCursor::moveIntoVariation(MoveId moveId)
{
    Q_ASSERT(variations().contains(moveId));
    m_currentBoard->doMove(m_nodes[moveId].move);
    m_currentNode = moveId;
}

MoveId GameCursor::addMove(const Move& move)
{
    Node node;
    MoveId previousNode = m_currentNode;

    node.nextNode = NO_MOVE;
    node.previousNode = m_currentNode;
    node.parentNode = m_nodes[m_currentNode].parentNode;
    node.move = move;
    if (move.isDummyMove())
    {
        node.SetPly(plyNumber());
    }
    else
    {
        node.SetPly(plyNumber() + 1);
    }
    m_nodes.append(node);
    m_currentNode = m_nodes.size() - 1;
    m_nodes[previousNode].nextNode = m_currentNode;
    m_currentBoard->doMove(move);
    return m_currentNode;
}

MoveId GameCursor::addVariation(const Move& move)
{
    auto previousNode = m_currentNode;
    auto saveNextNode = m_nodes[m_currentNode].nextNode;
    auto node = addMove(move);
    m_nodes[m_currentNode].parentNode = previousNode;
    m_nodes[previousNode].variations.append(node);
    m_nodes[previousNode].nextNode = saveNextNode;
    return node;
}

bool GameCursor::isRemoved(MoveId moveId) const
{
    moveId = makeNodeIndex(moveId);
    if (moveId <= ROOT_NODE)
        return false;
    return m_nodes[moveId].Removed();
}

void GameCursor::remove(MoveId moveId, QList<MoveId>* removed)
{
    auto node = makeNodeIndex(moveId);
    if (node <= ROOT_NODE)
        return;

    if (removed)
    {
        removed->append(node);
    }
    for (auto v: qAsConst(m_nodes[node].variations))
    {
        remove(v, removed);
    }
    remove(m_nodes[node].nextNode, removed);

    MoveId prevNode = m_nodes[node].previousNode;
    MoveId prntNode = m_nodes[node].parentNode;
    if ((prevNode >= ROOT_NODE) && (prntNode != prevNode))
    {
        // Do not delete main variation above sub variation
        m_nodes[prevNode].nextNode = NO_MOVE;
    }
    m_nodes[node].remove();
}

void GameCursor::truncateFrom(MoveId moveId, QList<MoveId>* removed)
{
    auto node = makeNodeIndex(moveId);
    if (node == NO_MOVE)
        return;
    remove(m_nodes[node].nextNode, removed);
    for (auto v: qAsConst(m_nodes[node].variations))
    {
        remove(v, removed);
    }
}

void GameCursor::truncateUpto(MoveId moveId, QList<MoveId>* removed)
{
    Q_UNUSED(removed);
    // TODO: figure why truncated nodes are not marked (mistake?)
    auto node = makeNodeIndex(moveId);
    if (node <= ROOT_NODE)
        return;

    if(atLineStart())
    {
        backward();
        forward();
    }

    auto save = m_currentNode;
    Node firstNode;
    firstNode.nextNode = m_currentNode;
    firstNode.SetPly(m_nodes[m_currentNode].Ply() - 1);
    // Keep variation if truncating main line
    if(m_nodes[m_nodes[m_currentNode].previousNode].nextNode == m_currentNode)
    {
        firstNode.variations = m_nodes[m_nodes[m_currentNode].previousNode].variations;
        foreach(MoveId var, firstNode.variations)
        {
            reparentVariation(var, 0);
            m_nodes[var].previousNode = 0;
        }
    }
    m_nodes[0] = firstNode;
    m_nodes[m_currentNode].previousNode = 0;
    backward();
    m_startingBoard = *m_currentBoard;
    // TODO: looks like restoring is redundant
    moveToId(save);
}

void GameCursor::promoteVariation(MoveId variation)
{
    auto save = m_currentNode;

    // Find first move of the variation
    while(!atLineStart(variation))
    {
        variation = m_nodes[variation].previousNode;
    }
    MoveId parent = m_nodes[variation].parentNode;

    // Link old main line to parent
    reparentVariation(m_nodes[parent].nextNode, parent);
    // Link new main line to parent's parent
    reparentVariation(variation, m_nodes[parent].parentNode);

    // Swap main line and the variation
    int index = m_nodes[parent].variations.indexOf(variation);
    qSwap(m_nodes[parent].nextNode, m_nodes[parent].variations[index]);
    moveToId(save);
}

bool GameCursor::canMoveVariationUp(MoveId moveId) const
{
    if (isMainline())
        return false;

    auto variation = variationNumber(moveId);
    auto parentNode = m_nodes[moveId].parentNode;

    auto& vars = m_nodes[parentNode].variations;
    int i = vars.indexOf(variation);
    return i > 0;
}

bool GameCursor::canMoveVariationDown(MoveId moveId) const
{
    if (isMainline())
        return false;

    auto variation = variationNumber(moveId);
    auto parentNode = m_nodes[moveId].parentNode;

    auto& vars = m_nodes[parentNode].variations;
    int i = vars.indexOf(variation);
    return 0 <= i && i + 1 < vars.size();
}

bool GameCursor::moveVariationUp(MoveId moveId)
{
    if (isMainline())
        return false;

    auto variation = variationNumber(moveId);
    auto parentNode = m_nodes[moveId].parentNode;

    auto& vars = m_nodes[parentNode].variations;
    int i = vars.indexOf(variation);
    auto possible = i > 0;
    if (possible)
    {
        vars.swapItemsAt(i, i - 1);
    }
    return possible;
}

bool GameCursor::moveVariationDown(MoveId moveId)
{
    if (isMainline())
        return false;

    auto variation = variationNumber(moveId);
    auto parentNode = m_nodes[moveId].parentNode;

    auto& vars = m_nodes[parentNode].variations;
    int i = vars.indexOf(variation);
    auto possible = 0 <= i && i + 1 < vars.size();
    if (possible)
    {
        vars.swapItemsAt(i, i + 1);
    }
    return possible;
}

bool GameCursor::removeVariation(MoveId variation)
{
    // don't remove whole game
    if(variation == ROOT_NODE)
    {
        return false;
    }
    MoveId parentNode = m_nodes[variation].parentNode;
    remove(variation);
    moveToId(parentNode);

    QList<MoveId> &vars = m_nodes[m_currentNode].variations;
    int n = vars.indexOf(variation);
    vars.removeAt(n);
    return true;
}

void GameCursor::removeVariations()
{
    for(int i = 0; i < m_nodes.size(); ++i)
    {
        while (!m_nodes[i].variations.empty())
        {
            removeVariation(m_nodes[i].variations.at(0));
        }
    }
}

void GameCursor::removeNullLines()
{
    auto iw = m_nodes.begin();
    for (; iw != m_nodes.end(); ++iw)
    {
        auto& node = *iw;
        if (node.Removed()) continue;
        if (node.move.isNullMove())
        {
            if (node.nextNode == NO_MOVE)
            {
                int self = iw-m_nodes.begin();
                if (node.parentNode == node.previousNode)
                {
                    // This is the first move of an empty variation
                    MoveId parentNode = m_nodes[self].parentNode;
                    remove(self);
                    QList<MoveId> &vars = m_nodes[parentNode].variations;
                    int n = vars.indexOf(self);
                    vars.removeAt(n);
                 }
                else
                {
                    MoveId previousNode = m_nodes[self].previousNode;
                    QList<MoveId> &vars = m_nodes[previousNode].variations;
                    if (vars.isEmpty())
                    {
                        // This is an empty move at the end of a line
                        m_nodes[previousNode].nextNode = NO_MOVE;
                        node.remove();
                    }
                    else
                    {
                        // There are siblings - swap with first sibling and remove
                        MoveId parentNode = m_nodes[self].parentNode;
                        int variation = vars.at(0);
                        reparentVariation(variation, parentNode);
                        node.remove();
                        m_nodes[previousNode].nextNode = variation;
                        vars.removeAt(0);
                    }
                }
            }
        }
    }
}

void GameCursor::reparentVariation(MoveId variation, MoveId parent)
{
    if (variation == NO_MOVE)
        return;
    for(auto node = variation; node != NO_MOVE; node = m_nodes[node].nextNode)
    {
        m_nodes[node].parentNode = parent;
    }
}

void GameCursor::clearDummyNodes()
{
    auto iw = m_nodes.begin();
    for (; iw != m_nodes.end(); ++iw)
    {
        auto& node = *iw;
        if (node.Removed()) continue;
        if (node.move.isDummyMove())
        {
            MoveId pre = node.previousNode;
            MoveId nxt = node.nextNode;

            if (nxt>=0)
            {
                if (pre>=0)
                {
                    m_nodes[pre].nextNode = nxt;
                }
                m_nodes[nxt].previousNode = pre;
                node.remove();
            }
            else
            {
                node.move.setNullMove();
                node.SetPly(node.Ply()+1);
            }
        }
    }
}

QMap<MoveId, MoveId> GameCursor::compact()
{
    QMap<MoveId,MoveId> renames;
    // map NO_MOVE for simplicity
    renames[NO_MOVE] = NO_MOVE;

    // arena ends
    auto ib = m_nodes.cbegin(), ie = m_nodes.cend();
    // read iterator
    auto ir = ib;
    // write iterator
    auto iw = m_nodes.begin();
    // keep track of indexes corresponding to `ir` and `iw`
    MoveId src = 0, dst = 0;
    for (; ir != ie; ++ir, ++src)
    {
        auto& node = *ir;
        // skip removed nodes
        if (node.Removed())
            continue;
        // move data
        *iw = *ir;
        // note rename
        renames[src] = dst;
        // update write iterator
        ++iw;
        ++dst;
    }
    // shrink m_nodes
    m_nodes.erase(iw, m_nodes.end());

    // update links
    for (auto& node: m_nodes)
    {
        node.nextNode = renames[node.nextNode];
        node.previousNode = renames[node.previousNode];
        node.parentNode = renames[node.parentNode];
        for (auto& v: node.variations)
        {
            v = renames[v];
        }
        node.variations.removeAll(NO_MOVE);
        node.variations.removeAll(ROOT_NODE);
    }
    m_currentNode = renames[m_currentNode];
    return renames;
}

MoveId GameCursor::findPosition(const BoardX& position) const
{
    MoveId current = 0;
    BoardX currentBoard(m_startingBoard);

    for(;;)
    {
        if(currentBoard == position && currentBoard.positionIsSame(position))
        {
            return current;
        }

        current = m_nodes[current].nextNode;
        if(current == NO_MOVE || !position.canBeReachedFrom(currentBoard))
        {
            return NO_MOVE;
        }

        currentBoard.doMove(m_nodes[current].move);
    }
    return NO_MOVE;
}

void GameCursor::dumpMoveNode(MoveId moveId) const
{
    if(moveId == CURRENT_MOVE)
    {
        moveId = m_currentNode;
    }
    if(moveId != NO_MOVE)
    {
        qDebug() << "Move Id : " << moveId;
        qDebug() << "   Next node   : " << m_nodes.at(moveId).nextNode;
        qDebug() << "   Prev node   : " << m_nodes.at(moveId).previousNode;
        qDebug() << "   Parent node : " << m_nodes.at(moveId).parentNode;
        qDebug() << "   Deleted     : " << m_nodes.at(moveId).Removed();
        qDebug() << "   # Variations: " << m_nodes.at(moveId).variations.size();
        qDebug() << "   Variations  : " << m_nodes.at(moveId).variations;
        qDebug() << "   Move        : " << m_nodes.at(moveId).move.toAlgebraic()
                 << " (" << m_nodes.at(moveId).move.rawMove()
                 << ", " << m_nodes.at(moveId).move.rawUndo()
                 << ")";
    }
}
