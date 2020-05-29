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
#include "ecopositions.h"
#include "gamex.h"
#include "tags.h"

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

template <typename T>
void applyRenames(QMap<MoveId, T>& map, const QMap<MoveId, MoveId>& renames)
{
    QMap<MoveId, T> tmp;
    for (auto src: map.keys())
    {
        auto dst = renames.value(src, NO_MOVE);
        if (dst != NO_MOVE)
        {
            tmp[dst] = map[src];
        }
    }
    qSwap(map, tmp);
}

MoveTree::MoveTree()
    : m_currentBoard(nullptr)
    , mountRefCount(0)
    , m_nodes()
    , m_currentNode(0)
    , m_startPly(0)
    , m_startingBoard()
{
    mountBoard();
}

MoveTree::MoveTree(const MoveTree& rhs)
    : m_currentBoard(nullptr)
    , mountRefCount(0)
    , m_nodes(rhs.m_nodes)
    , m_currentNode(rhs.m_currentNode)
    , m_startPly(rhs.m_startPly)
    , m_startingBoard(rhs.m_startingBoard)
{
    mountBoard();
}

MoveTree& MoveTree::operator=(const MoveTree& rhs)
{
    m_nodes = rhs.m_nodes;
    m_currentNode = rhs.m_currentNode;
    m_startPly = rhs.m_startPly;
    m_startingBoard = rhs.m_startingBoard;
    if (m_currentBoard && rhs.m_currentBoard)
    {
        *m_currentBoard = *rhs.m_currentBoard;
    }
    return *this;
}

MoveTree::~MoveTree()
{
    if (mountRefCount)
    {
        delete m_currentBoard;
        m_currentBoard = nullptr;
    }
}

void MoveTree::mountBoard()
{
    ++mountRefCount;
    if (mountRefCount == 1)
    {
        m_currentBoard = new BoardX;
    }
}

void MoveTree::unmountBoard()
{
    if (mountRefCount > 0)
    {
        --mountRefCount;
        if (mountRefCount == 0)
        {
            delete m_currentBoard;
            m_currentBoard = nullptr;
        }
    }
}

void MoveTree::clear()
{
    m_nodes.clear();
    m_startPly = 0;

    m_startingBoard.setStandardPosition();
    if (m_currentBoard)
    {
        m_currentNode = ROOT_NODE;
        *m_currentBoard = m_startingBoard;
    }
    else
    {
        m_currentNode = NO_MOVE;
    }
    m_nodes.append(Node());
}

void MoveTree::setInitialBoard(const QString& fen, bool chess960)
{
    m_startingBoard.setChess960(chess960);
    m_startingBoard.fromFen(fen);
    if (m_currentBoard)
    {
        *m_currentBoard = m_startingBoard;
    }
    m_startPly = (m_startingBoard.moveNumber() - 1) * 2 + (m_startingBoard.toMove() == Black);
}


MoveId MoveTree::makeNodeIndex(MoveId moveId) const
{
    if (moveId == CURRENT_MOVE)
    {
        moveId = m_currentNode;
    }
    bool rangeOk = 0 <= moveId && moveId < m_nodes.size();
    if (!rangeOk || m_nodes[moveId].Removed())
    {
        return NO_MOVE;
    }
    return moveId;
}

Move MoveTree::move(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if(node != NO_MOVE)
    {
        return m_nodes[node].move;
    }
    return Move();
}

int MoveTree::variationCount(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if(node != NO_MOVE)
    {
        return m_nodes[node].variations.size();
    }
    return 0;
}

const QList<MoveId>& MoveTree::variations() const
{
    return m_nodes[m_currentNode].variations;
}

bool MoveTree::isMainline(MoveId moveId) const
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

MoveId MoveTree::mainLineMove() const
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

bool MoveTree::atLineStart(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if(node == NO_MOVE)
    {
        return false;
    }
    return (m_nodes[node].previousNode == m_nodes[node].parentNode)
        || m_nodes[node].previousNode == 0;
}

bool MoveTree::atLineEnd(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if (node == NO_MOVE)
    {
        return false;
    }
    return m_nodes[node].nextNode == NO_MOVE;
}

bool MoveTree::atGameStart(MoveId moveId) const
{
    return (makeNodeIndex(moveId) == ROOT_NODE);
}

bool MoveTree::atGameEnd(MoveId moveId) const
{
    return (atLineEnd(moveId) && isMainline(moveId));
}

int MoveTree::plyCount() const
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

int MoveTree::plyNumber(MoveId moveId) const
{
    MoveId node = makeNodeIndex(moveId);
    if(node != NO_MOVE)
    {
        return m_nodes[node].Ply();
    }
    return 0;
}

int MoveTree::moveNumber(MoveId moveId) const
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

int MoveTree::countMoves() const
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

MoveId MoveTree::variationStartMove(MoveId variation) const
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

MoveId MoveTree::variationNumber(MoveId moveId) const
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

bool MoveTree::variationHasSiblings(MoveId variation) const
{
    variation = makeNodeIndex(variation);
    if(variation == NO_MOVE)
    {
        return false;
    }
    if(isMainline(variation))
    {
        return false;
    }
    while(!atLineStart(variation))
    {
        variation = m_nodes[variation].previousNode;
    }
    MoveId parent = m_nodes[variation].parentNode;
    return (variationCount(parent) > 1);
}

bool MoveTree::moveToId(MoveId moveId, QString* algebraicMoveList)
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

int MoveTree::forward(int count)
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

int MoveTree::backward(int count)
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

bool MoveTree::moveToLineEnd()
{
    return forward(999) != 0;
}

bool MoveTree::moveToStart()
{
    m_currentNode = 0;
    *m_currentBoard = m_startingBoard;
    return true;
}

bool MoveTree::moveToEnd()
{
    // Move out of variations to mainline
    while(m_nodes[m_currentNode].parentNode != NO_MOVE)
    {
        moveToId(m_nodes[m_currentNode].parentNode);
    }
    // Now move forward to the end of the game
    return moveToLineEnd();
}

void MoveTree::moveIntoVariation(MoveId moveId)
{
    Q_ASSERT(variations().contains(moveId));
    m_currentBoard->doMove(m_nodes[moveId].move);
    m_currentNode = moveId;
}

MoveId MoveTree::addMove(const Move& move)
{
    Node node;
    MoveId previousNode = m_currentNode;

    node.nextNode = NO_MOVE;
    node.previousNode = m_currentNode;
    node.parentNode = m_nodes[m_currentNode].parentNode;
    node.move = move;
    node.SetPly(plyNumber() + 1);
    m_nodes.append(node);
    m_currentNode = m_nodes.size() - 1;
    m_nodes[previousNode].nextNode = m_currentNode;
    m_currentBoard->doMove(move);
    return m_currentNode;
}

MoveId MoveTree::addVariation(const Move& move)
{
    auto previousNode = m_currentNode;
    auto saveNextNode = m_nodes[m_currentNode].nextNode;
    auto node = addMove(move);
    m_nodes[m_currentNode].parentNode = previousNode;
    m_nodes[previousNode].variations.append(node);
    m_nodes[previousNode].nextNode = saveNextNode;
    return node;
}

void MoveTree::remove(MoveId moveId, QList<MoveId>* removed)
{
    auto node = makeNodeIndex(moveId);
    if (node <= ROOT_NODE)
        return;

    if (removed)
    {
        removed->append(node);
    }
    for (auto v: m_nodes[node].variations)
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

void MoveTree::truncateFrom(MoveId moveId, QList<MoveId>* removed)
{
    auto node = makeNodeIndex(moveId);
    if (node == NO_MOVE)
        return;
    remove(m_nodes[node].nextNode, removed);
    for (auto v: m_nodes[node].variations)
    {
        remove(v, removed);
    }
}

void MoveTree::truncateUpto(MoveId moveId, QList<MoveId>* removed)
{
    // TODO: figure why truncated modes are not marked (mistake?)
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

void MoveTree::promoteVariation(MoveId variation)
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

bool MoveTree::canMoveVariationUp(MoveId moveId) const
{
    if (isMainline())
        return false;

    auto variation = variationNumber(moveId);
    auto parentNode = m_nodes[moveId].parentNode;

    auto& vars = m_nodes[parentNode].variations;
    int i = vars.indexOf(variation);
    return i > 0;
}

bool MoveTree::canMoveVariationDown(MoveId moveId) const
{
    if (isMainline())
        return false;

    auto variation = variationNumber(moveId);
    auto parentNode = m_nodes[moveId].parentNode;

    auto& vars = m_nodes[parentNode].variations;
    int i = vars.indexOf(variation);
    return 0 <= i && i + 1 < vars.size();
}

bool MoveTree::moveVariationUp(MoveId moveId)
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
        vars.swap(i, i - 1);
    }
    return possible;
}

bool MoveTree::moveVariationDown(MoveId moveId)
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
        vars.swap(i, i + 1);
    }
    return possible;
}

bool MoveTree::removeVariation(MoveId variation)
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
    vars.removeAt(vars.indexOf(variation));
    return true;
}

void MoveTree::removeVariations()
{
    for(int i = 0; i < m_nodes.size(); ++i)
    {
        while (!m_nodes[i].variations.empty())
        {
            removeVariation(m_nodes[i].variations.at(0));
        }
    }
}

void MoveTree::reparentVariation(MoveId variation, MoveId parent)
{
    if (variation == NO_MOVE)
        return;
    for(auto node = variation; node != NO_MOVE; node = m_nodes[node].nextNode)
    {
        m_nodes[node].parentNode = parent;
    }
}

QMap<MoveId, MoveId> MoveTree::compact()
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
        node.variations.removeAt(NO_MOVE);
        node.variations.removeAt(ROOT_NODE);
    }
    m_currentNode = renames[m_currentNode];
    return renames;
}

MoveId MoveTree::findPosition(const BoardX& position) const
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

static const char strSquareNames[64][3] =
{
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

const QStringList GameX::s_specList = QStringList() << s_emt << s_clk << s_egt << s_csl << s_cal;

GameX::GameX()
    : QObject()
    , m_moves()
{
    clearTags();
    clear();
}

GameX::GameX(const GameX& game)
    : QObject()
    , m_moves(game.m_moves)
{
    *this = game;
}

GameX& GameX::operator=(const GameX& game)
{
    if(this != &game)
    {
        clearTags();
        clear();
        //assign non pointer variables
        m_tags = game.m_tags;
        //m_tags.detach();
        m_variationStartAnnotations = game.m_variationStartAnnotations;
        //m_variationStartAnnotations.detach();
        m_annotations = game.m_annotations;
        m_nags = game.m_nags;
        //m_annotations.detach();
        //m_moveNodes.detach();
        m_moves = game.m_moves;
        if (m_moves.currentBoard() && !game.m_moves.currentBoard())
        {
            moveToStart();
        }
    }
    return *this;
}

GameX::~GameX()
{
}

void GameX::copyFromGame(const GameX& g)
{
    *this = g;
    emit signalGameModified(true,*this,tr("Copy game"));
}

MoveId GameX::dbAddMove(const Move& move, const QString& annotation, NagSet nags)
{
    auto node = m_moves.addMove(move);
    if(!annotation.isEmpty())
    {
        dbSetAnnotation(annotation, node);
    }
    if (!nags.empty())
    {
        m_nags[node] = nags;
    }
    return node;
}

MoveId GameX::addMove(const Move& move, const QString& annotation, NagSet nags)
{
    GameX state = *this;
    dbAddMove(move,annotation,nags);
    dbIndicateAnnotationsOnBoard();
    emit signalGameModified(true,state,tr("Add move"));
    return m_moves.currMove();
}

MoveId GameX::dbAddSanMove(const QString& sanMove, const QString& annotation, NagSet nags)
{
    Move move = m_moves.currentBoard()->parseMove(sanMove);
    if(move.isLegal() || move.isNullMove())
    {
        return dbAddMove(move, annotation, nags);
    }
    // qDebug() << sanMove << " : " << move.toAlgebraicDebug() << " is illegal in position " << board().toHumanFen() << " / " << board().toFen();
    return NO_MOVE;
}


MoveId GameX::addMove(const QString& sanMove, const QString& annotation, NagSet nags)
{
    Move move = m_moves.currentBoard()->parseMove(sanMove);
    if(move.isLegal() || move.isNullMove())
    {
        return addMove(move, annotation, nags);
    }
    return NO_MOVE;
}

MoveId GameX::addMoveFrom64Char(const QString &qcharboard)
{
    QStringList l = qcharboard.split(' ');
    if (l.size() < 30) return NO_MOVE;
    Char64Relation relation = (Char64Relation) l[C64_GAME_RELATION].toInt();
    QString s=l[C64_PP_LAST_MOVE];
    s.remove('+');
    s.remove('#');

    QString t = l[C64_ELAPSED_TIME_LAST_MOVE];
    t.remove("(");
    t.remove(")");
    QStringList tl = t.split(':');
    QString emt;
    if (tl.size()>=2)
    {
        emt = QString("[%emt 0:%1:%2]").arg(tl[0],-2,'0').arg(tl[1],-2,'0');
    }

    if (relation == C64_REL_PLAY_OPPONENT_MOVE)
    {
        if (s=="none")
        {
            return NO_MOVE;
        }
        else
        {
            if (!emt.isEmpty())
            {
                setAnnotation(emt);
            }
            return CURRENT_MOVE;
        }
    }
    else
    {
        Color thisMoveColor = (l[C64_COLOR_TO_MOVE]=="W") ? Black : White;
        if (thisMoveColor == board().toMove())
        {
            MoveId moveId = addMove(s,emt);
            return moveId;
        }
        else
        {
            return NO_MOVE;
        }
    }
}

bool GameX::mergeNode(GameX& otherGame)
{
    SaveRestoreMove saveThis(*this);
    SaveRestoreMove saveOther(otherGame);

    QString ann;
    NagSet nags;
    otherGame.forward();
    QString san = otherGame.moveToSan(MoveOnly, PreviousMove, CURRENT_MOVE, &ann, &nags);
    bool retVal = (NO_MOVE != dbAddSanMove(san, ann, nags));
    return retVal;
}

bool GameX::mergeVariations(GameX& otherGame)
{
    QList<MoveId> variationList = otherGame.variations();
    bool ok = true;
    if(!variationList.empty())
    {
        MoveId otherCurrent = otherGame.currentMove();
        for(QList<MoveId>::iterator iter = variationList.begin(); iter != variationList.end(); ++iter)
        {
            otherGame.enterVariation(*iter);
            if(!mergeAsVariation(otherGame))
            {
                ok = false;
            }
            otherGame.dbMoveToId(otherCurrent);
            if(!ok)
            {
                break;
            }
        }
    }
    return ok;
}

bool GameX::mergeAsMainline(GameX& otherGame)
{
    SaveRestoreMove saveNode(*this);

    QString ann;
    NagSet nags;

    QString san = otherGame.moveToSan(MoveOnly, PreviousMove, CURRENT_MOVE, &ann, &nags);
    if(NO_MOVE != dbAddSanMove(san, ann, nags))
    {
        while(!otherGame.atLineEnd())
        {
            forward();
            if(!mergeNode(otherGame))
            {
                return false;
            }
            mergeVariations(otherGame);
            otherGame.forward();
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool GameX::mergeAsVariation(GameX& otherGame)
{
    SaveRestoreMove saveNode(*this);

    QString ann;
    NagSet nags;

    QString san = otherGame.moveToSan(MoveOnly, PreviousMove, CURRENT_MOVE, &ann, &nags);
    if(NO_MOVE != dbAddSanVariation(san, ann, nags))
    {
        while(!otherGame.atLineEnd())
        {
            forward();
            if(!mergeNode(otherGame))
            {
                return false;
            }
            mergeVariations(otherGame);
            otherGame.forward();
        }
    }
    else
    {
        return false;
    }
    return true;
}

MoveId GameX::findMergePoint(const GameX& otherGame)
{
    MoveId prevNode = NO_MOVE;
    MoveId trailNode = NO_MOVE;
    MoveId otherMergeNode = NO_MOVE;
    bool found = false;
    do
    {
        if(NO_MOVE == (otherMergeNode = otherGame.model().findPosition(board())))
        {
            if(trailNode != NO_MOVE)
            {
                found = true;
                otherMergeNode = trailNode;
                break;
            }
        }
        else
        {
            prevNode = trailNode;
            trailNode = otherMergeNode;
        }
    }
    while(forward());

    if(!found && atLineEnd())
    {
        // Both games are identical up to the end of this game
        // Need to go one move back in the game to merge
        otherMergeNode = prevNode;
    }

    if(otherMergeNode == NO_MOVE)
    {
        if(otherGame.m_moves.capacity() > 0)
        {
            otherMergeNode = 0;
        }
    }

    // otherMergeNode points to the move before the game diverges
    // If the complete games needs to be merged, it points to node 0

    return otherMergeNode;
}

void GameX::dbMergeWithGame(const GameX& g)
{
    MoveId saveNode = m_moves.currMove();
    GameX otherGame = g;
    otherGame.moveToEnd();

    // Set the game information on the last move so that it is merged into this game
    QString white = otherGame.tag(TagNameWhite);
    QString black = otherGame.tag(TagNameBlack);
    QString event = otherGame.eventInfo();
    QString shortDescription;
    if (!(white.isEmpty() && black.isEmpty() && event.isEmpty()))
    {
        if (white.isEmpty()) white = "?";
        if (black.isEmpty()) black = "?";
        shortDescription = QString("%1-%2 %3").arg(white).arg(black).arg(event);
        otherGame.dbSetAnnotation(shortDescription);
    }

    MoveId otherMergeNode = findMergePoint(otherGame);

    if(otherMergeNode != NO_MOVE)
    {
        backward();
        // merge othergame starting with otherMergeNode into variation starting from m_currentNode
        otherGame.dbMoveToId(otherMergeNode);
        otherGame.forward();
        if(atLineEnd())
        {
            mergeAsMainline(otherGame);
        }
        else
        {
            mergeAsVariation(otherGame);
        }
        otherGame.dbMoveToId(otherMergeNode);
        mergeVariations(otherGame);
    }

    // undo changes
    if(saveNode != m_moves.currMove())
    {
        dbMoveToId(saveNode);
    }
    compact();
}

void GameX::mergeWithGame(const GameX& g)
{
    GameX state = *this;
    dbMergeWithGame(g);
    emit signalGameModified(true,state,tr("Merge game"));
}

bool GameX::positionRepetition(const BoardX& b)
{
    int repCount = 1;
    while(backward())
    {
        if (board() == b)
        {
            repCount++;
            if (repCount >= 3) break;
        }
    }
    return repCount >= 3;
}

bool GameX::positionRepetition3(const BoardX& b) const
{
    GameX g = *this;
    return g.positionRepetition(b);
}

bool GameX::insufficientMaterial(const BoardX& b) const
{
    return b.insufficientMaterial();
}

QString GameX::eventInfo() const
{
    QString result;
    QString site = tag(TagNameSite).left(30).remove("?");
    QString event = tag(TagNameEvent).left(30).remove("?");
    if(!site.isEmpty())
    {
        result.append(site);
        if(tag("Round") != "?")
        {
            result.append(QString(" (%1)").arg(tag("Round")));
        }
        if(!event.isEmpty())
        {
            result.append(", ");
        }
    }
    result.append(event);
    if(!tag("Date").startsWith("?"))
    {
        if(result.length() > 4)
        {
            result.append(", ");
        }
        result.append(tag(TagNameDate).remove(".??"));
    }
    return result;
}

bool GameX::currentNodeHasVariation(Square from, Square to) const
{
    if(m_moves.currMove() == NO_MOVE)
    {
        return false;
    }

    const QList<MoveId>& vs = currentVariations();
    QList<MoveId>::const_iterator i;
    for(i = vs.constBegin(); i != vs.constEnd(); ++i)
    {
        Move m = move(*i);
        if(m.from() == from && m.to() == to)
        {
            return true;
        }
    }
    return false;
}

const QList<MoveId>& GameX::currentVariations() const
{
    return m_moves.m_nodes[m_moves.currMove()].variations;
}

bool GameX::currentNodeHasMove(Square from, Square  to) const
{
    if(currentNodeHasVariation(from, to))
    {
        return true;
    }
    MoveId node = m_moves.m_nodes[m_moves.currMove()].nextNode;
    if(node == NO_MOVE)
    {
        return false;
    }
    Move m = m_moves.m_nodes[node].move;
    return (m.from() == from && m.to() == to);
}

bool GameX::hasNextMove() const
{
    MoveId node = m_moves.m_nodes[m_moves.currMove()].nextNode;
    return (node != NO_MOVE);
}

bool GameX::findNextMove(Move m)
{
    return findNextMove(m.from(),m.to(),m.isPromotion() ? pieceType(m.promotedPiece()) : None);
}

// does the next main move or one of the variations go from square from to square to
// if so make it on the board
bool GameX::findNextMove(Square from, Square to, PieceType promotionPiece)
{
    MoveId node;
    node = m_moves.m_nodes[m_moves.currMove()].nextNode;
    if(node != NO_MOVE)
    {
        Move m = m_moves.m_nodes[node].move ;
        if(m.from() == from && m.to() == to &&
                ((promotionPiece == None) || ((m.isPromotion() && (pieceType(m.promotedPiece()) == promotionPiece)))))
        {
            forward();
            return true;
        }
        else
        {
            QList<MoveId> vs = m_moves.m_nodes[m_moves.currMove()].variations;
            QList<MoveId>::iterator i;
            for(i = vs.begin(); i != vs.end(); ++i)
            {
                Move m = move(*i);
                if(m.from() == from && m.to() == to &&
                        ((promotionPiece == None) || ((m.isPromotion() && (pieceType(m.promotedPiece()) == promotionPiece)))))
                {
                    dbMoveToId(*i);
                    return true;
                }
            }
        }
    }
    return false;
}


bool GameX::replaceMove(const Move& move, const QString& annotation, NagSet nags, bool bReplace)
{
    MoveId node;
    GameX state = *this;

    node = m_moves.m_nodes[m_moves.currMove()].nextNode;
    if(node == NO_MOVE)
    {
        dbAddMove(move, annotation, nags);
        emit signalGameModified(true,state,tr("Add move"));
        return true;
    }

    //replace node data with new move
    m_moves.moveAt(node) = move;
    if (!nags.empty())
    {
        m_nags[node] = nags;
    }
    else
    {
        m_nags.remove(node);
    }
    dbSetAnnotation(annotation, node);

    //remove any following nodes after replaced move by disconnecting them from the tree
    forward();
    if(bReplace)
    {
        truncateVariation();
    }
    else
    {
        truncateVariationAfterNextIllegalPosition();
        compact();
    }
    backward();
    emit signalGameModified(true,state,tr("Replace move"));

    return true;
}

bool GameX::replaceMove(const QString& sanMove)
{
    return replaceMove(m_moves.currentBoard()->parseMove(sanMove), QString(), NagSet(), true);
}

bool GameX::insertMove(Move m)
{
    return replaceMove(m, annotation(), nags(), false);
}

MoveId GameX::addLine(const Move::List& moveList, const QString& annotation)
{
    GameX state = *this;
    MoveId retVal = dbAddLine(moveList, annotation);
    dbIndicateAnnotationsOnBoard();
    if (retVal != NO_MOVE)
    {
        emit signalGameModified(true, state, tr("Add line"));
    }
    return retVal;
}

MoveId GameX::addVariation(const Move& move, const QString& annotation, NagSet nags)
{
    GameX state = *this;
    MoveId retVal = dbAddVariation(move, annotation, nags);
    dbIndicateAnnotationsOnBoard();
    if (retVal != NO_MOVE)
    {
        emit signalGameModified(true, state, tr("Add variation"));
    }
    return retVal;
}

MoveId GameX::addVariation(const Move::List& moveList, const QString& annotation)
{
    GameX state = *this;
    MoveId retVal = dbAddVariation(moveList, annotation);
    dbIndicateAnnotationsOnBoard();
    if (retVal != NO_MOVE)
    {
        emit signalGameModified(true, state, tr("Add variation"));
    }
    return retVal;
}

MoveId GameX::addVariation(const QString& sanMove, const QString& annotation, NagSet nags)
{
    GameX state = *this;
    MoveId retVal = dbAddSanVariation(sanMove, annotation, nags);
    dbIndicateAnnotationsOnBoard();
    if (retVal != NO_MOVE)
    {
        emit signalGameModified(true, state, tr("Add variation"));
    }
    return retVal;
}

MoveId GameX::dbAddVariation(const Move& move, const QString& annotation, NagSet nags)
{
    auto node = m_moves.addVariation(move);
    if(!annotation.isEmpty())
    {
        dbSetAnnotation(annotation, node);
    }
    if (!nags.empty())
    {
        m_nags[node] = nags;
    }
    return node;
}

MoveId GameX::dbAddLine(const Move::List& moveList, const QString& annotation)
{
    if(moveList.isEmpty())
    {
        return NO_MOVE;
    }
    MoveId currentPosition = currentMove();
    for(int i = 0; i < moveList.count(); ++i)
    {
        dbAddMove(moveList[i]);
        forward();
    }
    if(!annotation.isEmpty())
    {
        dbSetAnnotation(annotation);
    }
    dbMoveToId(currentPosition);
    return currentPosition;
}

MoveId GameX::dbAddVariation(const Move::List& moveList, const QString& annotation)
{
    if(moveList.isEmpty())
    {
        return NO_MOVE;
    }
    MoveId currentPosition = currentMove();
    MoveId varStart;
    int start = 1;
    if(atLineEnd() && atGameStart())
    {
        Move oldMove = move();
        backward();
        varStart = dbAddVariation(oldMove);
        start = 0;
    }
    else
    {
        varStart = dbAddVariation(moveList.first());
    }

    dbMoveToId(varStart);
    for(int i = start; i < moveList.count(); ++i)
    {
        dbAddMove(moveList[i]);
        forward();
    }
    if(!annotation.isEmpty())
    {
        dbSetAnnotation(annotation);
    }
    dbMoveToId(currentPosition);
    return varStart;
}

MoveId GameX::dbAddSanVariation(const QString& sanMove, const QString& annotation, NagSet nags)
{
    Move move = m_moves.currentBoard()->parseMove(sanMove);
    if(move.isLegal() || move.isNullMove())
    {
        return dbAddVariation(move, annotation, nags);
    }
    return NO_MOVE;
}

bool GameX::promoteVariation(MoveId variation)
{
    if(isMainline(variation))
    {
        return false;
    }
    GameX state = *this;
    m_moves.promoteVariation(variation);
    emit signalGameModified(true, state, tr("Promote variation"));
    return true;
}

bool GameX::removeVariation(MoveId variation)
{
    GameX state = *this;
    bool success = m_moves.removeVariation(variation);
    if (success)
    {
        compact();
        emit signalGameModified(true, state, tr("Remove variation"));
    }
    return success;
}

void GameX::truncateVariationAfterNextIllegalPosition()
{
    SaveRestoreMove saveNode(*this);
    QList<MoveId> variationList = variations();
    for(QList<MoveId>::iterator iter = variationList.begin(); iter != variationList.end(); ++iter)
    {
        SaveRestoreMove saveNodeLoop(*this);
        enterVariation(*iter);
        truncateVariationAfterNextIllegalPosition();
    }
    QString san = moveToSan();
    GameX g = *this;
    if(NO_MOVE == g.dbAddSanMove(san, QString(), NagSet()))
    {
        MoveId node = m_moves.m_nodes[m_moves.currMove()].nextNode;
        removeNode(node);
    }
    else
    {
        if(forward())
        {
            truncateVariationAfterNextIllegalPosition();
        }
    }
}

void GameX::dbTruncateVariation(Position position)
{
    if(position == AfterMove)
    {
        m_moves.truncateFrom(CURRENT_MOVE);
    }
    else if(position == BeforeMove && m_moves.currMove() != ROOT_NODE)
    {
        m_moves.truncateUpto(CURRENT_MOVE);
        const auto& board = m_moves.initialBoard();
        if (board != BoardX::standardStartBoard)
        {
            m_tags[TagNameFEN] = board.toFen();
            m_tags[TagNameSetUp] = "1";
        }
    }
    compact();
}

void GameX::truncateVariation(Position position)
{
    GameX state = *this;
    dbTruncateVariation(position);
    emit signalGameModified(true, state, tr("Truncate variation"));
}

const BoardX& GameX::board() const
{
    return *m_moves.currentBoard();
}

QString GameX::toFen() const
{
    return m_moves.currentBoard()->toFen();
}

QString GameX::toHumanFen() const
{
    return m_moves.currentBoard()->toHumanFen();
}

void GameX::setGameComment(const QString& gameComment)
{
    setAnnotation(gameComment, 0);
}

void GameX::removeVariationsDb()
{
    m_moves.removeVariations();
    compact();
}

void GameX::removeVariations()
{
    GameX state = *this;
    removeVariationsDb();
    emit signalGameModified(true, state, tr("Remove variations"));
}

void GameX::removeCommentsDb()
{
    m_variationStartAnnotations.clear();
    m_annotations.clear();
    m_nags.clear();
    compact();
}

void GameX::removeTimeCommentsFromMap(AnnotationMap& map)
{
    QRegExp tan("\\[%(egt|emt|clk)\\s*(\\d?\\d:\\d?\\d:\\d\\d)\\]");
    AnnotationMap::iterator i;
    for (i = map.begin(); i != map.end(); ++i)
    {
        QString text = i.value();
        text = text.remove(tan);
        map[i.key()] = text;
    }
}

void GameX::removeTimeCommentsDb()
{
    removeTimeCommentsFromMap(m_annotations);
}

void GameX::removeComments()
{
    GameX state = *this;
    removeCommentsDb();
    emit signalGameModified(true, state, tr("Remove comments"));
}

void GameX::removeTimeComments()
{
    GameX state = *this;
    removeTimeCommentsDb();
    emit signalGameModified(true, state, tr("Remove time comments"));
}

Result GameX::result() const
{
    if(m_tags[TagNameResult] == "1-0")
    {
        return WhiteWin;
    }
    else if(m_tags[TagNameResult] == "1/2-1/2")
    {
        return Draw;
    }
    else if(m_tags[TagNameResult] == "0-1")
    {
        return BlackWin;
    }
    else
    {
        return ResultUnknown;
    }
}

int GameX::resultAsInt() const
{
    if(m_tags[TagNameResult] == "1-0")
    {
        return +1;
    }
    else if(m_tags[TagNameResult] == "1/2-1/2")
    {
        return 0;
    }
    else if(m_tags[TagNameResult] == "0-1")
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

bool GameX::setAnnotation(QString annotation, MoveId moveId, Position position)
{
    GameX state = *this;
    if (dbSetAnnotation(annotation, moveId, position))
    {
        dbIndicateAnnotationsOnBoard();
        emit signalGameModified(true, state, tr("Set annotation"));
        return true;
    }
    return false;
}

bool GameX::editAnnotation(QString annotation, MoveId moveId, Position position)
{
    GameX state = *this;
    QString spec = specAnnotations(moveId);
    annotation.append(spec);
    if (dbSetAnnotation(annotation, moveId, position))
    {
        dbIndicateAnnotationsOnBoard();
        emit signalGameModified(true, state, "Edit annotation");
        return true;
    }
    return false;
}

bool GameX::dbAppendAnnotation(QString a, MoveId moveId, Position position)
{
    QString s = annotation();
    s.append(a);
    return dbSetAnnotation(s, moveId, position);
}

bool GameX::dbPrependAnnotation(QString a, MoveId moveId, Position position)
{
    QString s = annotation();
    s.prepend(a);
    return dbSetAnnotation(s, moveId, position);
}

bool GameX::dbSetAnnotation(QString annotation, MoveId moveId, Position position)
{
    MoveId node = m_moves.makeNodeIndex(moveId);
    if(node == NO_MOVE)
    {
        return false;
    }

    int moves;
    int comments;
    int nags;
    moveCount(&moves, &comments, &nags);

    annotation.remove('}'); // Just make sure a comment does not destroy the database

    if(position == AfterMove)
    {
        if(annotation.isEmpty())
        {
            m_annotations.remove(node);
        }
        else
        {
            m_annotations[node] = annotation;
        }
    }
    else if(canHaveStartAnnotation(node))  	// Pre-move comment
    {
        if (node == 0)
        {
            setGameComment(annotation);
        }
        else
        {
            if(annotation.isEmpty() && (node > 1)) // Do not remove empty comment
            {
                m_variationStartAnnotations.remove(node);
            }
            else
            {
                m_variationStartAnnotations[node] = annotation;
            }
        }
    }
    return true;
}

bool GameX::setSquareAnnotation(QString squareAnnotation, MoveId moveId)
{
    squareAnnotation = squareAnnotation.trimmed();

    MoveId node = m_moves.makeNodeIndex(moveId);
    if(node == NO_MOVE)
    {
        return false;
    }

    QString s = annotation(moveId);
    s.remove(QRegExp(s_csl));

    if(!squareAnnotation.isEmpty())
    {
        s.append(QString("[%csl %1]").arg(squareAnnotation));
    }
    dbSetAnnotation(s, moveId);
    indicateAnnotationsOnBoard(moveId);
    return true;
}

bool GameX::appendSquareAnnotation(Square s, QChar colorCode)
{
    GameX state = *this;
    QString newAnnot;
    QString annot = squareAnnotation();
    QString oldAnnot = annot;
    QString sq = strSquareNames[s];
    if(annot.isEmpty())
    {
        if(colorCode != QChar(0))
        {
            newAnnot = QString("%1%2").arg(colorCode).arg(sq);
        }
    }
    else
    {
        annot.replace(QRegExp(QString(",.") + sq), ""); 	// if not first annotation remove annotation with preceding comma
        annot.replace(QRegExp(QString(".") + sq + ","), ""); // if first annotation remove annotation with trailing comma
        annot.replace(QRegExp(QString(".") + sq), ""); 	// if only annotation remove annotation
        if(colorCode != QChar(0))
        {
            if(annot.isEmpty())
            {
                newAnnot = QString("%1%2").arg(colorCode).arg(sq);	// If only annotation don't add comma
            }
            else
            {
                newAnnot = QString("%1,%2%3").arg(annot).arg(colorCode).arg(sq);	// if not only annotation add comma
            }
        }
        else
        {
            if(!annot.isEmpty())
            {
                newAnnot = annot;
            }
        }
        newAnnot.replace(" ,", " ");
        newAnnot.replace(",,", ",");
        if(newAnnot.endsWith("'"))
        {
            newAnnot.truncate(newAnnot.length() - 1);
        }
    }
    setSquareAnnotation(newAnnot.trimmed());
    emit signalGameModified(true, state, tr("Colorize square"));
    return true;
}

bool GameX::appendArrowAnnotation(Square dest, Square src, QChar colorCode)
{
    if((src == InvalidSquare) || (dest == InvalidSquare))
    {
        return false;
    }
    GameX state = *this;
    QString newAnnot;
    QString annot = arrowAnnotation();
    QString sqSrc = strSquareNames[src];
    QString sqDest = strSquareNames[dest];
    if(annot.isEmpty())
    {
        if(colorCode != QChar(0))
        {
            newAnnot = QString("%1%2%3").arg(colorCode).arg(sqSrc).arg(sqDest);
        }
    }
    else
    {
        annot.replace(QRegExp(QString(",.") + sqSrc + sqDest), ""); 	// if not first annotation remove annotation with preceding comma
        annot.replace(QRegExp(QString(".") + sqSrc + sqDest + ","), ""); // if first annotation remove annotation with trailing comma
        annot.replace(QRegExp(QString(".") + sqSrc + sqDest), ""); 	// if only annotation remove annotation

        if(colorCode != QChar(0))
        {
            newAnnot = QString("%1,%2%3%4").arg(annot).arg(colorCode).arg(sqSrc).arg(sqDest);
        }
        else
        {
            if(!annot.isEmpty())
            {
                newAnnot = annot;
            }
        }
        newAnnot.replace(" ,", " ");
        newAnnot.replace(",,", ",");
        if(newAnnot.endsWith("'"))
        {
            newAnnot.truncate(newAnnot.length() - 1);
        }
    }
    setArrowAnnotation(newAnnot);
    emit signalGameModified(true, state, tr("Paint arrow"));
    return true;
}

QString GameX::squareAnnotation(MoveId moveId) const
{
    QString s = specAnnotation(QRegExp(s_csl), moveId);
    return s;
}

bool GameX::setArrowAnnotation(QString arrowAnnotation, MoveId moveId)
{
    arrowAnnotation = arrowAnnotation.trimmed();

    MoveId node = m_moves.makeNodeIndex(moveId);
    if(node == NO_MOVE)
    {
        return false;
    }

    QString s = annotation(moveId);
    s.remove(QRegExp(s_cal));

    if(!arrowAnnotation.isEmpty())
    {
        s.append(QString("[%cal %1]").arg(arrowAnnotation));
    }
    dbSetAnnotation(s, moveId);
    indicateAnnotationsOnBoard(moveId);
    return true;
}

QString GameX::arrowAnnotation(MoveId moveId) const
{
    QString s = specAnnotation(QRegExp(s_cal), moveId);
    return s;
}

QString GameX::specAnnotation(const QRegExp& r, MoveId moveId) const
{
    MoveId node = m_moves.makeNodeIndex(moveId);
    if(node == NO_MOVE)
    {
        return QString("");
    }

    QString annotation = m_annotations[node];
    if(annotation.isNull())
    {
        return QString("");
    }

    int pos = r.indexIn(annotation);
    if(pos >= 0)
    {
        return r.cap(2);
    }

    return "";
}

QString GameX::timeAnnotation(MoveId moveId, Position position) const
{
    if (position == BeforeMove)
    {
        MoveId node = m_moves.makeNodeIndex(moveId);
        if (node>ROOT_NODE)
        {
            moveId = m_moves.m_nodes.at(node).previousNode;
        }
        else return "";
    }

    QString s = specAnnotation(QRegExp(s_tan), moveId);
    s = s.trimmed();
    return s;
}

QString GameX::annotation(MoveId moveId, Position position) const
{
    MoveId node = m_moves.makeNodeIndex(moveId);

    if ((position == AfterMove) || (node == 0))
    {
        return m_annotations.value(node,QString(""));
    }
    else
    {
        return m_variationStartAnnotations.value(node,QString(""));
    }
}

QString GameX::specAnnotations(MoveId moveId, Position position) const
{
    QString s = annotation(moveId, position);
    QString retval;
    foreach (QString sr, s_specList)
    {
        QRegExp r(sr);
        int pos = r.indexIn(s);
        if(pos >= 0)
        {
            retval += r.cap(0);
        }
    }
    return retval;
}

QString GameX::textAnnotation(MoveId moveId, Position position) const
{
    QString s = annotation(moveId, position);
    if (!s.isEmpty())
    {
        s.remove(QRegExp(s_tan));
        s.remove(QRegExp(s_can));
    }
    return s;
}

bool GameX::canHaveStartAnnotation(MoveId moveId) const
{
    MoveId node = m_moves.makeNodeIndex(moveId);
    return atLineStart(moveId) || atGameStart(m_moves.m_nodes[node].previousNode);
}

bool GameX::dbAddNag(Nag nag, MoveId moveId)
{
    MoveId node = m_moves.makeNodeIndex(moveId);
    if ((node != NO_MOVE) && (nag != NullNag))
    {
        m_nags[node].addNag(nag);
        return true;
    }
    return false;
}

bool GameX::addNag(Nag nag, MoveId moveId)
{
    MoveId node = m_moves.makeNodeIndex(moveId);
    if ((node != NO_MOVE) && (nag != NullNag))
    {
        GameX state = *this;
        dbAddNag(nag,moveId);
        emit signalGameModified(true, state, tr("Add nag"));
        return true;
    }
    return false;
}

bool GameX::setNags(NagSet nags, MoveId moveId)
{
    MoveId node = m_moves.makeNodeIndex(moveId);
    if(node != NO_MOVE)
    {
        GameX state = *this;
        if (!nags.empty())
        {
            m_nags[moveId] = nags;
        }
        else
        {
            m_nags.remove(moveId);
        }
        emit signalGameModified(true, state, tr("Set nags"));
        return true;
    }
    return false;
}

bool GameX::clearNags(MoveId moveId)
{
    return setNags(NagSet(), moveId);
}

NagSet GameX::nags(MoveId moveId) const
{
    static NagSet empty;
    MoveId node = m_moves.makeNodeIndex(moveId);
    return m_nags.value(node, empty);
}

void GameX::moveCount(int* moves, int* comments, int* nags) const
{
    *moves = m_moves.countMoves();
    *comments = m_annotations.size() + m_variationStartAnnotations.size();
    if (nags)
    {
        *nags = 0;
        for (auto value: m_nags)
        {
            if (!value.empty())
            {
                *nags += 1;
            }
        }
    }
}

bool GameX::isEmpty() const
{
    int moves;
    int comments;
    moveCount(&moves, &comments);
    bool gameIsEmpty = ((moves+comments) == 0);
    return gameIsEmpty;
}

void GameX::moveVariationUp(MoveId moveId)
{
    GameX state = *this;
    if (m_moves.moveVariationUp(moveId))
    {
        emit signalGameModified(true, state, tr("Move variation"));
    }
}

void GameX::moveVariationDown(MoveId moveId)
{
    GameX state = *this;
    if (m_moves.moveVariationDown(moveId))
    {
        emit signalGameModified(true, state, tr("Move variation"));
    }
}

void GameX::enumerateVariations(MoveId moveId, char a)
{
    if(isMainline())
    {
        return;
    }

    MoveId node = m_moves.makeNodeIndex(moveId);
    if(node != NO_MOVE)
    {
        GameX state = *this;
        MoveId parentNode = m_moves.m_nodes[node].parentNode;
        QList <MoveId>& v = m_moves.m_nodes[parentNode].variations;
        if (!v.empty())
        {
            for(int i = 0; i < v.size(); ++i)
            {
                QString oldAnnotation = annotation(v[i], GameX::BeforeMove);
                oldAnnotation.remove(QRegExp("^.\\)"));
                QString s = QString("%1) %2").arg(QChar(a + i)).arg(oldAnnotation).trimmed();
                dbSetAnnotation(s, v[i], GameX::BeforeMove);
            }
            emit signalGameModified(true, state, tr("Enumerate variations"));
        }
    }
}

MoveId GameX::lastMove() const
{
    MoveId moveId = 0;
    while((m_moves.m_nodes[moveId].nextNode != NO_MOVE))
    {
        moveId = m_moves.m_nodes[moveId].nextNode;
    }
    return moveId;
}

void GameX::dbIndicateAnnotationsOnBoard()
{
    auto moveId = m_moves.currMove();

    QString annotation = squareAnnotation(moveId);
    m_moves.currentBoard()->setSquareAnnotation(annotation);

    annotation = arrowAnnotation(moveId);
    m_moves.currentBoard()->setArrowAnnotation(annotation);
}

void GameX::indicateAnnotationsOnBoard(MoveId moveId)
{
    dbIndicateAnnotationsOnBoard();
    emit signalMoveChanged();
}

void GameX::moveToStart()
{
    m_moves.moveToStart();
    indicateAnnotationsOnBoard();
}

int GameX::moveByPly(int diff)
{
    if(diff > 0)
    {
        return forward(diff);
    }
    else
    {
        return backward(-diff);
    }
}

void GameX::moveToId(MoveId moveId)
{
    if (dbMoveToId(moveId))
    {
        indicateAnnotationsOnBoard();
    }
}

void GameX::moveToEnd()
{
    if (!m_moves.moveToEnd())
    {
        indicateAnnotationsOnBoard();
    }
}

void GameX::moveToLineEnd()
{
    // TODO: review this, looks wrong
    if (!m_moves.moveToEnd())
    {
        indicateAnnotationsOnBoard();
    }
}

int GameX::forward(int count)
{
    int moved = m_moves.forward(count);
    if (moved)
    {
        indicateAnnotationsOnBoard();
    }
    return moved;
}

int GameX::backward(int count)
{
    int moved = m_moves.backward(count);
    if(moved)
    {
        indicateAnnotationsOnBoard();
    }
    return moved;
}

void GameX::enterVariation(const MoveId& moveId)
{
    m_moves.moveIntoVariation(moveId);
    indicateAnnotationsOnBoard();
}

void GameX::removeNode(MoveId moveId)
{
    QList<MoveId> removed;
    m_moves.remove(moveId, &removed);
    for (auto node: removed)
    {
        m_annotations.remove(node);
        m_variationStartAnnotations.remove(node);
        m_nags.remove(node);
    }
}

void GameX::clear()
{
    m_moves.clear();
    m_variationStartAnnotations.clear();
    m_annotations.clear();
    m_nags.clear();
}

void GameX::clearTags()
{
    m_tags.clear();
}

QString GameX::tag(const QString& tag) const
{
    return m_tags[tag];
}

bool GameX::hasTag(const QString& tag) const
{
    return m_tags.contains(tag);
}

const TagMap& GameX::tags() const
{
    return m_tags;
}

void GameX::setTag(const QString& tag, const QString& value)
{
    m_tags[tag] = value;
}

void GameX::removeTag(const QString& tag)
{
    m_tags.remove(tag);
}

void GameX::setStartingBoard(const BoardX& startingBoard, QString text, bool chess960)
{
    GameX state = *this;
    dbSetStartingBoard(startingBoard.toFen(), chess960);
    emit signalGameModified(true, state, text);
}

void GameX::dbSetStartingBoard(const QString& fen, bool chess960)
{
    clear();
    dbSetChess960(chess960);
    m_moves.setInitialBoard(fen, chess960);
    if (m_moves.initialBoard() != BoardX::standardStartBoard)
    {
        m_tags[TagNameFEN] = fen;
        m_tags[TagNameSetUp] = "1";
    }
}

void GameX::dbSetResult(Result result)
{
    m_tags[TagNameResult] = resultString(result);
}

void GameX::setResult(Result result)
{
    GameX state = *this;
    dbSetResult(result);
    emit signalGameModified(true, state, tr("Set result"));
}

bool GameX::isChess960() const
{
    QString s = m_tags[TagNameVariant].toLower();
    return (s.startsWith("fischer") || s.endsWith("960"));
}

void GameX::dbSetChess960(bool b)
{
    if (b)
    {
        m_tags[TagNameVariant] = "Chess960";
    }
    else
    {
        removeTag(TagNameVariant);
    }
}

QString GameX::moveToSan(MoveStringFlags flags, NextPreviousMove nextPrevious, MoveId moveId,
                         QString* annots, NagSet* nagSet)
{
    MoveId node = m_moves.makeNodeIndex(moveId);
    if(node != NO_MOVE && nextPrevious == NextMove)
    {
        node = m_moves.m_nodes[node].nextNode;
    }
    if(node == NO_MOVE)
    {
        return QString();
    }

    MoveNode move;
    move = m_moves.m_nodes[node];
    if(!(move.move.isLegal() || move.move.isNullMove()))
    {
        return QString();
    }

    // Save current node
    MoveId saveNode = NO_MOVE;
    MoveId boardNode = m_moves.m_nodes[node].previousNode;
    if(boardNode != m_moves.currMove())
    {
        saveNode = m_moves.currMove();
        dbMoveToId(boardNode);
    }

    // Move number
    QString san;
    if(m_moves.currentBoard()->toMove() == Black && flags & BlackNumbers)
    {
        san += QString::number(moveNumber(node)) + "... ";
    }
    else if(m_moves.currentBoard()->toMove() == White && flags & WhiteNumbers)
    {
        san += QString::number(moveNumber(node)) + ". ";
    }

    // Move and SAN
    san += m_moves.currentBoard()->moveToSan(move.move, flags & TranslatePiece);
    if(flags & Nags)
    {
        san += nags(node).toString(NagSet::Simple);
    }

    if(nagSet)
    {
        *nagSet = nags(node);
    }
    if(annots)
    {
        *annots = annotation(node);
    }

    // Restore previous position
    if(saveNode != NO_MOVE)
    {
        dbMoveToId(saveNode);
    }

    return san;
}

void GameX::dumpMoveNode(MoveId moveId) const
{
    if(moveId == CURRENT_MOVE)
    {
        moveId = m_moves.currMove();
    }
    if(moveId != NO_MOVE)
    {
        qDebug() << "Move Id : " << moveId;
        qDebug() << "   Next node   : " << m_moves.m_nodes.at(moveId).nextNode;
        qDebug() << "   Prev node   : " << m_moves.m_nodes.at(moveId).previousNode;
        qDebug() << "   Parent node : " << m_moves.m_nodes.at(moveId).parentNode;
        qDebug() << "   Nags        : " << m_nags.value(moveId, NagSet()).toString(NagSet::Simple);
        qDebug() << "   Deleted     : " << m_moves.m_nodes.at(moveId).Removed();
        qDebug() << "   # Variations: " << m_moves.m_nodes.at(moveId).variations.size();
        qDebug() << "   Variations  : " << m_moves.m_nodes.at(moveId).variations;
        qDebug() << "   Move        : " << m_moves.m_nodes.at(moveId).move.toAlgebraic()
                 << " (" << m_moves.m_nodes.at(moveId).move.rawMove()
                 << ", " << m_moves.m_nodes.at(moveId).move.rawUndo()
                 << ")";
    }
}

void GameX::dumpAnnotations(MoveId moveId) const
{
    if(moveId == CURRENT_MOVE)
    {
        moveId = m_moves.currMove();
    }
    if(moveId != NO_MOVE)
    {
        qDebug() << "   Annotations";
        qDebug() << "     Text        : " << m_annotations.value(moveId);
        qDebug() << "     Start       : " << m_variationStartAnnotations.value(moveId);
    }
}


void GameX::dumpAllMoveNodes() const
{
    qDebug() << endl;
    qDebug() << "Current Node: " << m_moves.currMove();
    for(int i = 0; i < m_moves.m_nodes.size(); ++i)
    {
        dumpMoveNode(i);
        dumpAnnotations(i);
    }
    int moves, comments, nags;
    moveCount(&moves, &comments, &nags);
    qDebug() << "Moves: " << moves << " Comments: " << comments << " Nags: " << nags << endl;
    qDebug() << "----------------------------------" << endl;
}

void GameX::compact()
{
    auto renames = m_moves.compact();
    applyRenames(m_annotations, renames);
    applyRenames(m_variationStartAnnotations, renames);
    applyRenames(m_nags, renames);
}

QString GameX::ecoClassify() const
{
    //move to end of main line
    GameX g = *this;
    if (g.startingBoard() != BoardX::standardStartBoard)
    {
        if (g.isChess960())
        {
            return QString();
        }
    }
    g.m_moves.moveToEnd();

    //search backwards for the first eco position
    while(g.m_moves.backward())
    {
        QString eco;
        if (EcoPositions::isEcoPosition(g.board(),eco))
        {
            return eco;
        }
    }

    return QString();
}

bool GameX::isEcoPosition() const
{
    QString dummy;
    return (EcoPositions::isEcoPosition(board(),dummy));
}

void GameX::scoreMaterial(QList<double>& scores) const
{
    GameX g = *this;
    g.moveToStart();
    scores.clear();
    int score = g.board().ScoreMaterial();
    scores.append(score);
    while(g.forward())
    {
        int score = g.board().ScoreMaterial();
        scores.append(score);
    }
}

int GameX::isEqual(const GameX& game) const
{
    return ((m_moves.m_nodes == game.m_moves.m_nodes) &&
            (m_nags == game.m_nags) &&
            (m_annotations == game.m_annotations) &&
            (m_variationStartAnnotations == game.m_variationStartAnnotations));
}

int GameX::isBetterOrEqual(const GameX& game) const
{
    return ((m_moves.m_nodes.count() >= game.m_moves.m_nodes.count()) &&
            (m_annotations.count() >= game.m_annotations.count()) &&
            (m_variationStartAnnotations.count() >= game.m_variationStartAnnotations.count()));
}
