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
#include "game.h"
#include "tags.h"
#include "ecopositions.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER


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

const char* Game::s_emt = "\\[%emt\\s*(\\d:\\d\\d?:\\d\\d)\\]";
const char* Game::s_clk = "\\[%clk\\s*(\\d:\\d\\d?:\\d\\d)\\]";
const char* Game::s_egt = "\\[%egt\\s*(\\d:\\d\\d?:\\d\\d)\\]";
const char* Game::s_csl = "\\[%csl\\s*([^\\]]*)\\]";
const char* Game::s_cal = "\\[%cal\\s*([^\\]]*)\\]";

Game::Game() : QObject()
{
    mountRefCount = 0;
    m_currentBoard = 0;
    mountBoard();
    clearTags();
    clear();
}

Game::Game(const Game& game) : QObject()
{
    mountRefCount = 0;
    m_currentBoard = 0;
    mountBoard();
    *this = game;
}

Game& Game::operator=(const Game & game)
{
    if(this != &game)
    {
        clearTags();
        clear();
        //assign non pointer variables
        m_startingBoard = game.m_startingBoard;
        m_tags = game.m_tags;
        //m_tags.detach();
        m_variationStartAnnotations = game.m_variationStartAnnotations;
        //m_variationStartAnnotations.detach();
        m_annotations = game.m_annotations;
        //m_annotations.detach();
        m_startPly = game.m_startPly;
        m_moveNodes = game.m_moveNodes;
        //m_moveNodes.detach();
        m_currentNode = game.m_currentNode;
        if (m_currentBoard && game.m_currentBoard)
        {
            *m_currentBoard = *game.m_currentBoard;
        }
        else if (m_currentBoard)
        {
            moveToStart();
        }
    }
    return *this;
}

Game::~Game()
{
    if (mountRefCount)
    {
        delete m_currentBoard;
        m_currentBoard = 0;
    }
}

void Game::mountBoard()
{
    ++mountRefCount;
    if (mountRefCount == 1)
    {
        m_currentBoard = new Board;
    }
}

void Game::unmountBoard()
{
    if (mountRefCount > 0)
    {
        --mountRefCount;
        if (mountRefCount == 0)
        {
            delete m_currentBoard;
            m_currentBoard = 0;
        }
    }
}

void Game::copyFromGame(const Game& g)
{
    *this = g;
    emit signalGameModified(true,*this,tr("Copy game"));
}

MoveId Game::dbAddMove(const Move& move, const QString& annotation, NagSet nags)
{
    MoveNode node;
    MoveId previousNode = m_currentNode;

    node.nextNode = NO_MOVE;
    node.previousNode = m_currentNode;
    node.parentNode = m_moveNodes[m_currentNode].parentNode;
    node.move = move;
    node.nags = nags;
    node.SetPly(ply() + 1);
    m_moveNodes.append(node);
    m_currentNode = m_moveNodes.size() - 1;
    m_moveNodes[previousNode].nextNode = m_currentNode;
    if(!annotation.isEmpty())
    {
        dbSetAnnotation(annotation, m_currentNode);
    }
    m_currentBoard->doMove(move);
    return m_currentNode;
}

MoveId Game::addMove(const Move& move, const QString& annotation, NagSet nags)
{
    Game state = *this;
    dbAddMove(move,annotation,nags);
    dbIndicateAnnotationsOnBoard(m_currentNode);
    emit signalGameModified(true,state,tr("Add move"));
    return m_currentNode;
}

MoveId Game::dbAddMove(const QString& sanMove, const QString& annotation, NagSet nags)
{
    Move move = m_currentBoard->parseMove(sanMove);
    if(move.isLegal() || move.isNullMove())
    {
        return dbAddMove(move, annotation, nags);
    }
    // qDebug() << sanMove << " : " << move.toAlgebraicDebug() << " is illegal in position " << board().toHumanFen() << " / " << board().toFen();
    return NO_MOVE;
}


MoveId Game::addMove(const QString& sanMove, const QString& annotation, NagSet nags)
{
    Move move = m_currentBoard->parseMove(sanMove);
    if(move.isLegal() || move.isNullMove())
    {
        return addMove(move, annotation, nags);
    }
    return NO_MOVE;
}

MoveId Game::addMoveFrom64Char(const QString &qcharboard)
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

bool Game::mergeNode(Game& otherGame)
{
    SaveRestoreMove saveThis(*this);
    SaveRestoreMove saveOther(otherGame);

    QString ann;
    NagSet nags;
    otherGame.forward();
    QString san = otherGame.moveToSan(MoveOnly, PreviousMove, CURRENT_MOVE, &ann, &nags);

    bool retVal = (NO_MOVE != dbAddMove(san, ann, nags));
    return retVal;
}

bool Game::mergeVariations(Game& otherGame)
{
    QList<MoveId> variationList = otherGame.variations();
    bool ok = true;
    if(variationList.size())
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

bool Game::mergeAsMainline(Game& otherGame)
{
    SaveRestoreMove saveNode(*this);

    QString ann;
    NagSet nags;

    QString san = otherGame.moveToSan(MoveOnly, PreviousMove, CURRENT_MOVE, &ann, &nags);

    if(NO_MOVE != dbAddMove(san, ann, nags))
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

bool Game::mergeAsVariation(Game& otherGame)
{
    SaveRestoreMove saveNode(*this);

    QString ann;
    NagSet nags;

    QString san = otherGame.moveToSan(MoveOnly, PreviousMove, CURRENT_MOVE, &ann, &nags);

    if(NO_MOVE != dbAddVariation(san, ann, nags))
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

MoveId Game::findMergePoint(const Game& otherGame)
{
    MoveId prevNode = NO_MOVE;
    MoveId trailNode = NO_MOVE;
    MoveId otherMergeNode = NO_MOVE;
    bool found = false;
    do
    {
        if(NO_MOVE == (otherMergeNode = otherGame.findPosition(board())))
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
        if(otherGame.m_moveNodes.size() > 0)
        {
            otherMergeNode = 0;
        }
    }

    // otherMergeNode points to the move before the game diverges
    // If the complete games needs to be merged, it points to node 0

    return otherMergeNode;
}

void Game::dbMergeWithGame(const Game& g)
{
    MoveId saveNode = m_currentNode;
    Game otherGame = g;
    otherGame.moveToEnd();

    // Set the game information on the last move so that it is merged into this game
    QString white = otherGame.tag(TagNameWhite);
    QString black = otherGame.tag(TagNameBlack);
    QString event = otherGame.eventInfo();
    QString shortDescription = QString("%1-%2 %3").arg(white).arg(black).arg(event);
    otherGame.dbSetAnnotation(shortDescription);

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
    if(saveNode != m_currentNode)
    {
        dbMoveToId(saveNode);
    }
    compact();
}

void Game::mergeWithGame(const Game& g)
{
    Game state = *this;
    dbMergeWithGame(g);
    emit signalGameModified(true,state,tr("Merge game"));
}

bool Game::positionRepetition(const Board& b)
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

bool Game::positionRepetition3(const Board& b) const
{
    Game g = *this;
    return g.positionRepetition(b);
}

bool Game::insufficientMaterial(const Board& b) const
{
    return b.insufficientMaterial();
}

QString Game::eventInfo() const
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

bool Game::currentNodeHasVariation(Square from, Square to) const
{
    if(m_currentNode == NO_MOVE)
    {
        return false;
    }

    QList<MoveId> vs = m_moveNodes[m_currentNode].variations;
    QList<MoveId>::iterator i;
    for(i = vs.begin(); i != vs.end(); ++i)
    {
        Move m = move(*i);
        if(m.from() == from && m.to() == to)
        {
            return true;
        }
    }
    return false;
}

bool Game::currentNodeHasMove(Square from, Square  to) const
{
    if(currentNodeHasVariation(from, to))
    {
        return true;
    }
    MoveId node = m_moveNodes[m_currentNode].nextNode;
    if(node == NO_MOVE)
    {
        return false;
    }
    Move m = m_moveNodes[node].move;
    return (m.from() == from && m.to() == to);
}

bool Game::hasNextMove() const
{
    MoveId node = m_moveNodes[m_currentNode].nextNode;
    return (node != NO_MOVE);
}

bool Game::findNextMove(Move m)
{
    return findNextMove(m.from(),m.to(),m.isPromotion() ? pieceType(m.promotedPiece()) : None);
}

// does the next main move or one of the variations go from square from to square to
// if so make it on the board
bool Game::findNextMove(Square from, Square to, PieceType promotionPiece)
{
    MoveId node;
    node = m_moveNodes[m_currentNode].nextNode;
    if(node != NO_MOVE)
    {
        Move m = m_moveNodes[node].move ;
        if(m.from() == from && m.to() == to &&
                ((promotionPiece == None) || ((m.isPromotion() && (pieceType(m.promotedPiece()) == promotionPiece)))))
        {
            forward();
            return true;
        }
        else
        {
            QList<MoveId> vs = m_moveNodes[m_currentNode].variations;
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


bool Game::replaceMove(const Move& move, const QString& annotation, NagSet nags, bool bReplace)
{
    MoveId node;
    Game state = *this;

    node = m_moveNodes[m_currentNode].nextNode;
    if(node == NO_MOVE)
    {
        dbAddMove(move, annotation, nags);
        emit signalGameModified(true,state,tr("Add move"));
        return true;
    }

    //replace node data with new move
    m_moveNodes[node].move = move;
    m_moveNodes[node].nags = nags;
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

bool Game::replaceMove(const QString& sanMove)
{
    return replaceMove(m_currentBoard->parseMove(sanMove), QString(), NagSet(), true);
}

bool Game::insertMove(Move m)
{
    return replaceMove(m, annotation(), nags(), false);
}

MoveId Game::addVariation(const Move& move, const QString& annotation, NagSet nags)
{
    Game state = *this;
    MoveId retVal = dbAddVariation(move, annotation, nags);
    dbIndicateAnnotationsOnBoard(m_currentNode);
    if (retVal != NO_MOVE)
    {
        emit signalGameModified(true, state, tr("Add variation"));
    }
    return retVal;
}

MoveId Game::addVariation(const MoveList& moveList, const QString& annotation)
{
    Game state = *this;
    MoveId retVal = dbAddVariation(moveList, annotation);
    dbIndicateAnnotationsOnBoard(m_currentNode);
    if (retVal != NO_MOVE)
    {
        emit signalGameModified(true, state, tr("Add variation"));
    }
    return retVal;
}

MoveId Game::addVariation(const QString& sanMove, const QString& annotation, NagSet nags)
{
    Game state = *this;
    MoveId retVal = dbAddVariation(sanMove, annotation, nags);
    dbIndicateAnnotationsOnBoard(m_currentNode);
    if (retVal != NO_MOVE)
    {
        emit signalGameModified(true, state, tr("Add variation"));
    }
    return retVal;
}

MoveId Game::dbAddVariation(const Move& move, const QString& annotation, NagSet nags)
{
    MoveId previousNode = m_currentNode;
    MoveId saveNextNode = m_moveNodes[m_currentNode].nextNode;
    MoveId node = dbAddMove(move, annotation, nags);
    m_moveNodes[m_currentNode].parentNode = previousNode;
    m_moveNodes[previousNode].variations.append(node);
    m_moveNodes[previousNode].nextNode = saveNextNode;

    return (m_moveNodes.size() - 1);
}

MoveId Game::dbAddVariation(const MoveList& moveList, const QString& annotation)
{
    if(moveList.isEmpty())
    {
        return NO_MOVE;
    }
    MoveId currentPosition = currentMove();
    MoveId varStart;
    int start = 1;
    if(atLineEnd() && !!atGameStart())
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

MoveId Game::dbAddVariation(const QString& sanMove, const QString& annotation, NagSet nags)
{
    MoveId previousNode = m_currentNode;
    MoveId saveNextNode = m_moveNodes[m_currentNode].nextNode;
    MoveId node = dbAddMove(sanMove, annotation, nags);
    if(node == NO_MOVE)
    {
        return node;
    }

    m_moveNodes[m_currentNode].parentNode = previousNode;
    m_moveNodes[previousNode].variations.append(node);
    m_moveNodes[previousNode].nextNode = saveNextNode;

    return (m_moveNodes.size() - 1);
}

void Game::dbPromoteVariation(MoveId variation)
{
    SaveRestoreMove saveNode(*this);

    // Find first move of the variation
    while(!atLineStart(variation))
    {
        variation = m_moveNodes[variation].previousNode;
    }
    MoveId parent = m_moveNodes[variation].parentNode;

    // Link old main line to parent
    reparentVariation(m_moveNodes[parent].nextNode, parent);
    // Link new main line to parent's parent
    reparentVariation(variation, m_moveNodes[parent].parentNode);

    // Swap main line and the variation
    int index = m_moveNodes[parent].variations.indexOf(variation);
    qSwap(m_moveNodes[parent].nextNode, m_moveNodes[parent].variations[index]);
}

bool Game::promoteVariation(MoveId variation)
{
    if(isMainline(variation))
    {
        return false;
    }
    Game state = *this;
    dbPromoteVariation(variation);
    emit signalGameModified(true, state, tr("Promote variation"));
    return true;
}

bool Game::removeVariation(MoveId variation)
{
    // don't remove whole game
    if(!variation)
    {
        return false;
    }
    Game state = *this;
    MoveId parentNode = m_moveNodes[variation].parentNode;
    removeNode(variation);
    dbMoveToId(parentNode);

    QList<MoveId> &vars = m_moveNodes[m_currentNode].variations;
    vars.removeAt(vars.indexOf(variation));

    compact();
    emit signalGameModified(true, state, tr("Remove variation"));
    return true;
}

void Game::truncateVariationAfterNextIllegalPosition()
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
    Game g = *this;

    if(NO_MOVE == g.dbAddMove(san))
    {
        MoveId node = m_moveNodes[m_currentNode].nextNode;
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

void Game::dbTruncateVariation(Position position)
{
    if(position == AfterMove)
    {
        MoveId node = m_moveNodes[m_currentNode].nextNode;
        if (isMainline(node))
        {
            foreach(MoveId var, m_moveNodes[m_currentNode].variations)
            {
                removeNode(var);
            }
        }
        removeNode(node);
    }
    else if(position == BeforeMove && m_currentNode != 0)
    {
        if(atLineStart())
        {
            backward();
            forward();
        }
        SaveRestoreMove saveNode(*this);
        MoveNode firstNode;
        firstNode.nextNode = m_currentNode;
        firstNode.SetPly(m_moveNodes[m_currentNode].Ply() - 1);
        // Keep variation if truncating main line
        if(m_moveNodes[m_moveNodes[m_currentNode].previousNode].nextNode == m_currentNode)
        {
            firstNode.variations = m_moveNodes[m_moveNodes[m_currentNode].previousNode].variations;
            foreach(MoveId var, firstNode.variations)
            {
                reparentVariation(var, 0);
                m_moveNodes[var].previousNode = 0;
            }
        }
        m_moveNodes[0] = firstNode;
        m_moveNodes[m_currentNode].previousNode = 0;
        backward();
        m_startingBoard = *m_currentBoard;
        if(m_startingBoard != Board::standardStartBoard)
        {
            m_tags[TagNameFEN] = m_startingBoard.toFen();
            m_tags[TagNameSetUp] = "1";
        }
    }
    compact();
}

void Game::truncateVariation(Position position)
{
    Game state = *this;
    dbTruncateVariation(position);
    emit signalGameModified(true, state, tr("Truncate variation"));
}

const Board& Game::board() const
{
    return *m_currentBoard;
}

Board Game::startingBoard() const
{
    return m_startingBoard;
}

QString Game::toFen() const
{
    return m_currentBoard->toFen();
}

QString Game::toHumanFen() const
{
    return m_currentBoard->toHumanFen();
}

QString Game::gameComment() const
{
    return m_annotations.value(0, QString(""));
}

void Game::setGameComment(const QString& gameComment)
{
    setAnnotation(gameComment, 0);
}

void Game::removeVariations()
{
    Game state = *this;
    for(int i = 0; i < m_moveNodes.size(); ++i)
    {
        while (!m_moveNodes[i].variations.empty())
        {
            removeVariation(m_moveNodes[i].variations.at(0));
        }
    }
    compact();
    emit signalGameModified(true, state, tr("Remove variations"));
}

void Game::removeComments()
{
    Game state = *this;
    m_variationStartAnnotations.clear();
    m_annotations.clear();
    for(int i = 0; i < m_moveNodes.size(); ++i)
    {
        m_moveNodes[i].nags.clear();
    }
    compact();
    emit signalGameModified(true, state, tr("Remove comments"));
}

bool Game::isMainline(MoveId moveId) const
{
    if(moveId == 0)
    {
        return true;
    }
    MoveId node = nodeValid(moveId);
    if(node == NO_MOVE)
    {
        return false;
    }
    else
    {
        return m_moveNodes[node].parentNode == NO_MOVE;
    }
}

Result Game::result() const
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

int Game::resultAsInt() const
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

int Game::numberOfSiblings(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if(node == NO_MOVE)
    {
        return 0;
    }
    MoveId parentNode = m_moveNodes[node].parentNode;
    if(parentNode == NO_MOVE)
    {
        return 0;
    }
    return m_moveNodes[parentNode].variations.size();
}

bool Game::atLineStart(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if(node == NO_MOVE)
    {
        return false;
    }
    return (m_moveNodes[node].previousNode == m_moveNodes[node].parentNode)
           || m_moveNodes[node].previousNode == 0;
}

bool Game::atGameStart(MoveId moveId) const
{
    return (nodeValid(moveId) == 0);
}

bool Game::atGameEnd(MoveId moveId) const
{
    return (atLineEnd(moveId) && isMainline(moveId));
}

bool Game::atLineEnd(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if(node != NO_MOVE)
    {
        if(m_moveNodes[node].nextNode == NO_MOVE)
        {
            return true;
        }
    }

    return false;
}

bool Game::setAnnotation(QString annotation, MoveId moveId, Position position)
{
    Game state = *this;
    if (dbSetAnnotation(annotation, moveId, position))
    {
        dbIndicateAnnotationsOnBoard(currentMove());
        emit signalGameModified(true, state, tr("Set annotation"));
        return true;
    }
    return false;
}

bool Game::editAnnotation(QString annotation, MoveId moveId, Position position)
{
    Game state = *this;
    if (dbSetAnnotation(annotation, moveId, position))
    {
        dbIndicateAnnotationsOnBoard(currentMove());
        emit signalGameModified(true, state, "Edit annotation");
        return true;
    }
    return false;
}

bool Game::dbSetAnnotation(QString annotation, MoveId moveId, Position position)
{
    MoveId node = nodeValid(moveId);
    if(node == NO_MOVE)
    {
        return false;
    }

    int moves;
    int comments;
    int nags;
    moveCount(&moves, &comments, &nags);

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

bool Game::setSquareAnnotation(QString squareAnnotation, MoveId moveId)
{
    squareAnnotation = squareAnnotation.trimmed();

    MoveId node = nodeValid(moveId);
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
    return true;
}

bool Game::appendSquareAnnotation(Square s, QChar colorCode)
{
    Game state = *this;
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
    dbIndicateAnnotationsOnBoard(currentMove());
    emit signalGameModified(true, state, tr("Colorize square"));
    return true;
}

bool Game::appendArrowAnnotation(Square dest, Square src, QChar colorCode)
{
    if((src == InvalidSquare) || (dest == InvalidSquare))
    {
        return false;
    }
    Game state = *this;
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
    dbIndicateAnnotationsOnBoard(currentMove());
    emit signalGameModified(true, state, tr("Paint arrow"));
    return true;
}

QString Game::squareAnnotation(MoveId moveId) const
{
    QString s = specAnnotation(QRegExp(s_csl), moveId);
    return s;
}

bool Game::setArrowAnnotation(QString arrowAnnotation, MoveId moveId)
{
    arrowAnnotation = arrowAnnotation.trimmed();

    MoveId node = nodeValid(moveId);
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
    return true;
}

QString Game::arrowAnnotation(MoveId moveId) const
{
    QString s = specAnnotation(QRegExp(s_cal), moveId);
    return s;
}

QString Game::specAnnotation(const QRegExp& r, MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
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
        return r.cap(1);
    }

    return "";
}

QString Game::timeAnnotation(MoveId moveId, Position position) const
{
    if (position == BeforeMove)
    {
        MoveId node = nodeValid(moveId);
        if (node>ROOT_NODE)
        {
            moveId = m_moveNodes.at(node).previousNode;
        }
        else return "";
    }

    QString s = specAnnotation(QRegExp(s_clk), moveId);
    s = s.trimmed();
    if(s.isEmpty())
    {
        s = specAnnotation(QRegExp(s_egt), moveId);
    }
    if (s.isEmpty())
    {
        s = specAnnotation(QRegExp(s_emt), moveId);
    }
    return s;
}

QString Game::annotation(MoveId moveId, Position position) const
{
    MoveId node = nodeValid(moveId);

    if ((position == AfterMove) || (node == 0))
    {
        return m_annotations.value(node,QString(""));
    }
    else
    {
        return m_variationStartAnnotations.value(node,QString(""));
    }
}

QString Game::textAnnotation(MoveId moveId, Position position) const
{
    QString s;
    MoveId node = nodeValid(moveId);

    if ((position == AfterMove) || (node == 0))
    {
        s = m_annotations.value(node,QString(""));
    }
    else
    {
        s = m_variationStartAnnotations.value(node,QString(""));
    }

    s.remove(QRegExp(s_emt));
    s.remove(QRegExp(s_egt));
    s.remove(QRegExp(s_clk));
    s.remove(QRegExp(s_csl));
    s.remove(QRegExp(s_cal));

    return s;
}

bool Game::canHaveStartAnnotation(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    return atLineStart(moveId) || atGameStart(m_moveNodes[node].previousNode);
}

bool Game::dbAddNag(Nag nag, MoveId moveId)
{
    MoveId node = nodeValid(moveId);
    if ((node != NO_MOVE) && (nag != NullNag))
    {
        m_moveNodes[node].nags.addNag(nag);
        return true;
    }
    return false;
}

bool Game::addNag(Nag nag, MoveId moveId)
{
    MoveId node = nodeValid(moveId);
    if ((node != NO_MOVE) && (nag != NullNag))
    {
        Game state = *this;
        dbAddNag(nag,moveId);
        emit signalGameModified(true, state, tr("Add nag"));
        return true;
    }
    return false;
}

bool Game::setNags(NagSet nags, MoveId moveId)
{
    MoveId node = nodeValid(moveId);
    if(node != NO_MOVE)
    {
        Game state = *this;
        m_moveNodes[node].nags = nags;
        emit signalGameModified(true, state, tr("Set nags"));
        return true;
    }
    return false;
}

bool Game::clearNags(MoveId moveId)
{
    return setNags(NagSet(), moveId);
}

NagSet Game::nags(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if(node != NO_MOVE)
    {
        return m_moveNodes[node].nags;
    }
    return NagSet();
}

MoveId Game::nodeValid(MoveId moveId) const
{
    if(moveId == CURRENT_MOVE)
    {
        moveId = m_currentNode;
    }
    if((moveId >= 0) && (moveId < m_moveNodes.size()))
    {
        if(m_moveNodes[moveId].Removed())
        {
            return NO_MOVE;
        }
        return moveId;
    }
    return NO_MOVE;
}

void Game::moveCount(int* moves, int* comments, int* nags) const
{
    *moves = *comments = 0;
    if (nags) *nags = 0;

    MoveId node = 1;
    while((node = nodeValid(node)) != NO_MOVE)
    {
        *moves += 1;
        if (nags)
        {
            if(m_moveNodes[node].nags.count() != 0)
            {
                *nags += 1;
            }
        }
        node = m_moveNodes[node].nextNode;
    }
    // Count comments
    for(int i = 0; i < m_annotations.size(); ++i)
    {
        *comments += 1;
    }
    for(int i = 0; i < m_variationStartAnnotations.size(); ++i)
    {
        *comments += 1;
    }
}

bool Game::isEmpty() const
{
    int moves;
    int comments;
    moveCount(&moves, &comments);
    bool gameIsEmpty = ((moves+comments) == 0);
    return gameIsEmpty;
}

int Game::ply(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if(node != NO_MOVE)
    {
        return m_moveNodes[node].Ply();
    }
    return 0;
}

int Game::moveNumber(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if(node != NO_MOVE)
    {
        if(int plyNum = ply(node))
        {
            return (m_startPly + plyNum - 1) / 2 + 1;
        }
        return 0;
    }
    return -1;
}

int Game::plyCount() const
{
    int count = 0;
    MoveId node = 0;

    while(node != NO_MOVE)
    {
        ++count;
        node = m_moveNodes[node].nextNode;
    }
    // Counted one too much, because we have to start at zero
    // (an empty game has no node 1)
    return count - 1;
}

bool Game::canMoveVariationUp(MoveId moveId) const
{
    if(isMainline())
    {
        return false;
    }

    MoveId variation = variationNumber(moveId);
    MoveId parentNode = m_moveNodes[moveId].parentNode;

    const QList <MoveId>& v = m_moveNodes[parentNode].variations;
    int i = v.indexOf(variation);
    return (i > 0);
}

void Game::moveVariationUp(MoveId moveId)
{
    if(isMainline())
    {
        return;
    }

    MoveId variation = variationNumber(moveId);
    MoveId parentNode = m_moveNodes[moveId].parentNode;

    QList <MoveId>& v = m_moveNodes[parentNode].variations;
    int i = v.indexOf(variation);
    if(i > 0)
    {
        Game state = *this;
        v.swap(i, i - 1);
        emit signalGameModified(true, state, tr("Move variation"));
    }
}

bool Game::canMoveVariationDown(MoveId moveId) const
{
    if(isMainline())
    {
        return false;
    }

    MoveId variation = variationNumber(moveId);
    MoveId parentNode = m_moveNodes[moveId].parentNode;

    const QList <MoveId>& v = m_moveNodes[parentNode].variations;
    int i = v.indexOf(variation);
    return (i >= 0 && (i + 1) < v.count());
}

void Game::moveVariationDown(MoveId moveId)
{
    if(isMainline())
    {
        return;
    }

    MoveId variation = variationNumber(moveId);
    MoveId parentNode = m_moveNodes[moveId].parentNode;

    QList <MoveId>& v = m_moveNodes[parentNode].variations;
    int i = v.indexOf(variation);
    if(i >= 0 && (i + 1) < v.count())
    {
        Game state = *this;
        v.swap(i, i + 1);
        emit signalGameModified(true, state, tr("Move variation"));
    }
}

void Game::enumerateVariations(MoveId moveId, char a)
{
    if(isMainline())
    {
        return;
    }

    MoveId node = nodeValid(moveId);
    if(node != NO_MOVE)
    {
        Game state = *this;
        MoveId parentNode = m_moveNodes[node].parentNode;
        QList <MoveId>& v = m_moveNodes[parentNode].variations;
        if (v.size())
        {
            for(int i = 0; i < v.size(); ++i)
            {
                QString oldAnnotation = annotation(v[i], Game::BeforeMove);
                oldAnnotation.remove(QRegExp("^.\\)"));
                QString s = QString("%1) %2").arg(QChar(a + i)).arg(oldAnnotation).trimmed();
                dbSetAnnotation(s, v[i], Game::BeforeMove);
            }
            emit signalGameModified(true, state, tr("Enumerate variations"));
        }
    }
}

MoveId Game::variationNumber(MoveId moveId) const
{
    if(isMainline())
    {
        return 0;
    }
    MoveId node = nodeValid(moveId);
    if(node != NO_MOVE)
    {
        MoveId parentNode = m_moveNodes[node].parentNode;

        while(m_moveNodes[node].previousNode != parentNode)
        {
            node = m_moveNodes[node].previousNode;
        }
    }
    return node;
}

MoveId Game::mainLineMove() const
{
    MoveId node = nodeValid(m_currentNode);
    if(node != NO_MOVE)
    {
        bool dive = false;
        while(m_moveNodes[node].parentNode != NO_MOVE)
        {
            dive = true;
            node = m_moveNodes[node].parentNode;
        }
        if(dive)
        {
            node = m_moveNodes[node].nextNode;
        }
    }
    return node;
}

MoveId Game::variationStartMove(MoveId variation) const
{
    variation = nodeValid(variation);
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
        variation = m_moveNodes[variation].previousNode;
    }
    return variation;
}

MoveId Game::previousMove() const
{
    return m_moveNodes[m_currentNode].previousNode;
}

MoveId Game::nextMove() const
{
    return m_moveNodes[m_currentNode].nextNode;
}

MoveId Game::parentMove() const
{
    return m_moveNodes[m_currentNode].parentNode;
}

MoveId Game::currentMove() const
{
    return m_currentNode;
}

MoveId Game::lastMove() const
{
    MoveId moveId = 0;
    while((m_moveNodes[moveId].nextNode != NO_MOVE))
    {
        moveId = m_moveNodes[moveId].nextNode;
    }
    return moveId;
}

const QList<MoveId>& Game::variations() const
{
    return m_moveNodes[m_currentNode].variations;
}

int Game::variationCount(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if(node != NO_MOVE)
    {
        return m_moveNodes[node].variations.size();
    }
    return 0;
}

bool Game::variationHasSiblings(MoveId variation) const
{
    variation = nodeValid(variation);
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
        variation = m_moveNodes[variation].previousNode;
    }
    MoveId parent = m_moveNodes[variation].parentNode;
    return (variationCount(parent) > 1);
}

void Game::dbIndicateAnnotationsOnBoard(MoveId moveId)
{
    QString annotation = squareAnnotation(moveId);
    m_currentBoard->setSquareAnnotation(annotation);

    annotation = arrowAnnotation(moveId);
    m_currentBoard->setArrowAnnotation(annotation);
}

void Game::indicateAnnotationsOnBoard(MoveId moveId)
{
    dbIndicateAnnotationsOnBoard(moveId);
    emit signalMoveChanged();
}

void Game::moveToStart()
{
    m_currentNode = 0;
    *m_currentBoard = m_startingBoard;

    indicateAnnotationsOnBoard(m_currentNode);
}

int Game::moveByPly(int diff)
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

bool Game::dbMoveToId(MoveId moveId)
{
    moveId = nodeValid(moveId);
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
            moveStack.push(m_moveNodes[node].move);
            node = m_moveNodes[node].previousNode;
        }

        //reset the board, then make the moves on the stack to create the correct position
        m_currentNode = moveId;
        *m_currentBoard = m_startingBoard;
        while(!moveStack.isEmpty())
        {
            m_currentBoard->doMove(moveStack.pop());
        }
    }

    return true;
}

void Game::moveToId(MoveId moveId)
{
    if (dbMoveToId(moveId))
    {
        indicateAnnotationsOnBoard(moveId);
    }
}

Move Game::move(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if(node != NO_MOVE)
    {
        return m_moveNodes[node].move;
    }
    return Move();
}

bool Game::dbMoveToEnd()
{
    // Move out of variations to mainline
    while(m_moveNodes[m_currentNode].parentNode != NO_MOVE)
    {
        dbMoveToId(m_moveNodes[m_currentNode].parentNode);
    }
    // Now move forward to the end of the game
    return (!forward(999));
}

void Game::moveToEnd()
{
    if (!dbMoveToEnd())
    {
        indicateAnnotationsOnBoard(m_currentNode);
    }
}

int Game::forward(int count)
{
    int moved = 0;
    while((m_moveNodes[m_currentNode].nextNode != NO_MOVE) && (moved < count))
    {
        m_currentNode = m_moveNodes[m_currentNode].nextNode;
        ++moved;

        m_currentBoard->doMove(m_moveNodes[m_currentNode].move);
    }

    if(moved)
    {
        indicateAnnotationsOnBoard(m_currentNode);
    }

    return moved;
}

int Game::dbBackward(int count)
{
    int moved = 0;
    while((m_moveNodes[m_currentNode].previousNode >= 0) && (moved < count))
    {
        m_currentBoard->undoMove(m_moveNodes[m_currentNode].move);
        m_currentNode = m_moveNodes[m_currentNode].previousNode;
        ++moved;
    }
    return moved;
}

int Game::backward(int count)
{
    int moved = dbBackward(count);
    if(moved)
    {
        indicateAnnotationsOnBoard(m_currentNode);
    }
    return moved;
}

void Game::enterVariation(const MoveId& moveId)
{
    Q_ASSERT(variations().contains(moveId));
    m_currentBoard->doMove(m_moveNodes[moveId].move);
    m_currentNode = moveId;

    indicateAnnotationsOnBoard(m_currentNode);
}

void Game::removeNode(MoveId moveId)
{
    MoveId node = nodeValid(moveId);
    if(node > ROOT_NODE)
    {
        m_variationStartAnnotations.remove(node);
        m_annotations.remove(node);

        if(variationCount(node))
        {
            for(int i = 0; i < m_moveNodes[node].variations.size(); ++i)
            {
                removeNode(m_moveNodes[node].variations[i]);
            }
        }
        removeNode(m_moveNodes[node].nextNode);
        MoveId prevNode = m_moveNodes[node].previousNode;

        if(!m_moveNodes[prevNode].variations.contains(node))
        {
            m_moveNodes[prevNode].nextNode = NO_MOVE;
        }
        m_moveNodes[node].remove();
    }
}

void Game::clear()
{
    m_moveNodes.clear();
    m_variationStartAnnotations.clear();
    m_annotations.clear();

    m_startPly = 0;

    m_startingBoard.setStandardPosition();
    if (m_currentBoard)
    {
        m_currentNode = 0;
        *m_currentBoard = m_startingBoard;
    }
    else
    {
        m_currentNode = NO_MOVE;
    }

    m_moveNodes.append(MoveNode());
}

void Game::clearTags()
{
    m_tags.clear();
}

QString Game::tag(const QString& tag) const
{
    return m_tags[tag];
}

bool Game::hasTag(const QString& tag) const
{
    return m_tags.contains(tag);
}

const TagMap& Game::tags() const
{
    return m_tags;
}

void Game::setTag(const QString& tag, const QString& value)
{
    m_tags[tag] = value;
}

void Game::removeTag(const QString& tag)
{
    m_tags.remove(tag);
}

void Game::setStartingBoard(const Board& startingBoard, QString text, bool chess960)
{
    Game state = *this;
    dbSetStartingBoard(startingBoard.toFen(), chess960);
    emit signalGameModified(true, state, text);
}

void Game::dbSetStartingBoard(const QString& fen, bool chess960)
{
    clear();
    setChess960(chess960);
    m_startingBoard.setChess960(chess960);
    m_startingBoard.fromFen(fen);
    if(m_startingBoard != Board::standardStartBoard)
    {
        m_tags[TagNameFEN] = fen;
        m_tags[TagNameSetUp] = "1";
    }
    if (m_currentBoard)
    {
        *m_currentBoard = m_startingBoard;
    }
    m_startPly = (m_startingBoard.moveNumber() - 1) * 2 + (m_startingBoard.toMove() == Black);
}

void Game::dbSetResult(Result result)
{
    m_tags[TagNameResult] = resultString(result);
}

void Game::setResult(Result result)
{
    Game state = *this;
    dbSetResult(result);
    emit signalGameModified(true, state, tr("Set result"));
}

bool Game::isChess960() const
{
    QString s = m_tags[TagNameVariant].toLower();
    return (s.startsWith("fischer") || s.endsWith("960"));
}

void Game::dbSetChess960(bool b)
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

void Game::setChess960(bool b)
{
    Game state = *this;
    dbSetChess960(b);
    emit signalGameModified(true, state, tr("Set variant"));
}

QString Game::moveToSan(MoveStringFlags flags, NextPreviousMove nextPrevious, MoveId moveId,
                        QString* annots, NagSet* nagSet)
{
    MoveId node = nodeValid(moveId);
    if(node != NO_MOVE && nextPrevious == NextMove)
    {
        node = m_moveNodes[node].nextNode;
    }
    if(node == NO_MOVE)
    {
        return QString();
    }

    MoveNode move;
    move = m_moveNodes[node];
    if(!(move.move.isLegal() || move.move.isNullMove()))
    {
        return QString();
    }

    // Save current node
    MoveId saveNode = NO_MOVE;
    MoveId boardNode = m_moveNodes[node].previousNode;
    if(boardNode != m_currentNode)
    {
        saveNode = m_currentNode;
        dbMoveToId(boardNode);
    }

    // Move number
    QString san;
    if(m_currentBoard->toMove() == Black && flags & BlackNumbers)
    {
        san += QString::number(moveNumber(node)) + "... ";
    }
    else if(m_currentBoard->toMove() == White && flags & WhiteNumbers)
    {
        san += QString::number(moveNumber(node)) + ". ";
    }

    // Move and SAN
    san += m_currentBoard->moveToSan(move.move, flags & TranslatePiece);
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

void Game::dumpMoveNode(MoveId moveId) const
{
    if(moveId == CURRENT_MOVE)
    {
        moveId = m_currentNode;
    }
    if(moveId != NO_MOVE)
    {
        qDebug() << "Move Id : " << moveId;
        qDebug() << "   Next node   : " << m_moveNodes.at(moveId).nextNode;
        qDebug() << "   Prev node   : " << m_moveNodes.at(moveId).previousNode;
        qDebug() << "   Parent node : " << m_moveNodes.at(moveId).parentNode;
        qDebug() << "   Nags        : " << m_moveNodes.at(moveId).nags.toString(NagSet::Simple);
        qDebug() << "   Deleted     : " << m_moveNodes.at(moveId).Removed();
        qDebug() << "   # Variations: " << m_moveNodes.at(moveId).variations.size();
        qDebug() << "   Variations  : " << m_moveNodes.at(moveId).variations;
        qDebug() << "   Move        : " << m_moveNodes.at(moveId).move.toAlgebraic()
                 << " (" << m_moveNodes.at(moveId).move.rawMove()
                 << ", " << m_moveNodes.at(moveId).move.rawUndo()
                 << ")";
    }
}

void Game::dumpAnnotations(MoveId moveId) const
{
    if(moveId == CURRENT_MOVE)
    {
        moveId = m_currentNode;
    }
    if(moveId != NO_MOVE)
    {
        qDebug() << "   Annotations";
        qDebug() << "     Text        : " << m_annotations.value(moveId);
        qDebug() << "     Start       : " << m_variationStartAnnotations.value(moveId);
    }
}


void Game::dumpAllMoveNodes() const
{
    qDebug() << endl;
    qDebug() << "Current Node: " << m_currentNode;
    for(int i = 0; i < m_moveNodes.size(); ++i)
    {
        dumpMoveNode(i);
        dumpAnnotations(i);
    }
    int moves, comments, nags;
    moveCount(&moves, &comments, &nags);
    qDebug() << "Moves: " << moves << " Comments: " << comments << " Nags: " << nags << endl;
    qDebug() << "----------------------------------" << endl;    
}

MoveId Game::findPosition(const Board& position) const
{
    MoveId current = 0;
    Board currentBoard(m_startingBoard);

    for(;;)
    {
        if(currentBoard == position && currentBoard.positionIsSame(position))
        {
            return current;
        }

        current = m_moveNodes[current].nextNode;
        if(current == NO_MOVE || !position.canBeReachedFrom(currentBoard))
        {
            return NO_MOVE;
        }

        currentBoard.doMove(m_moveNodes[current].move);
    }
    return NO_MOVE;
}

void Game::compact()
{
    int oldSize = m_moveNodes.size();
    QList<MoveNode> moveNodes;
    QMap<MoveId,MoveId> oldIdNewIdMapping;
    QList<MoveId> removedNodes;

    for(MoveId i = 0; i < oldSize; ++i)
    {
        if(!m_moveNodes[i].Removed())
        {
            oldIdNewIdMapping[i] = moveNodes.size();
            moveNodes.append(m_moveNodes[i]);
        }
        else
        {
            removedNodes.push_back(i);
        }
    }

    foreach(MoveId m, removedNodes)
    {
        m_variationStartAnnotations.remove(m);
        m_annotations.remove(m);
    }

    AnnotationMap variationStartAnnotations;
    AnnotationMap annotations;

    foreach(MoveId key, oldIdNewIdMapping.keys())
    {
        MoveId n = oldIdNewIdMapping.value(key);
#define GAME_UPDATE_ANNOT(t,x) \
        if (x.contains(key)) \
        {\
            QString s = x.value(key);\
            t[n] = s;\
        }

        GAME_UPDATE_ANNOT(variationStartAnnotations, m_variationStartAnnotations);
        GAME_UPDATE_ANNOT(annotations, m_annotations);
    }

    // update nodes links to other nodes in shrinked list (prev, next, variations)
    for(MoveId i = 0, newSize = moveNodes.size(); i < newSize; ++i)
    {
        MoveNode& node = moveNodes[i];
#define GAME_UPDATE_MOVEID(aMoveId) if (aMoveId != NO_MOVE) aMoveId = oldIdNewIdMapping[aMoveId]
        GAME_UPDATE_MOVEID(node.nextNode);
        GAME_UPDATE_MOVEID(node.previousNode);
        GAME_UPDATE_MOVEID(node.parentNode);
        QList<MoveId>& vars = node.variations;
        for(int j = 0; j < vars.size(); ++j)
        {
            GAME_UPDATE_MOVEID(vars[j]);
        }
        vars.removeAll(NO_MOVE);
        vars.removeAll(0);
#undef GAME_UPDATE_MOVEID
    }

    m_variationStartAnnotations.clear();
    m_annotations.clear();

    m_variationStartAnnotations = variationStartAnnotations;
    m_annotations               = annotations;

    //m_variationStartAnnotations.detach();
    //m_annotations.detach();

    m_moveNodes.clear();
    m_moveNodes = moveNodes;
    m_currentNode = oldIdNewIdMapping[m_currentNode];
}

QString Game::ecoClassify() const
{
    //move to end of main line
    Game g = *this;
    if (g.startingBoard() != Board::standardStartBoard)
    {
        if (g.isChess960())
        {
            return QString();
        }
    }
    g.dbMoveToEnd();

    //search backwards for the first eco position
    while(g.dbBackward())
    {
        QString eco;
        if (EcoPositions::isEcoPosition(g.board(),eco))
        {
            return eco;
        }
    }

    return QString();
}

bool Game::isEcoPosition() const
{
    QString dummy;
    return (EcoPositions::isEcoPosition(board(),dummy));
}

void Game::scoreMaterial(QList<double>& scores) const
{
    Game g = *this;
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

void Game::reparentVariation(MoveId variation, MoveId parent)
{
    if(variation != NO_MOVE)
    {
        for(MoveId node = variation; node != NO_MOVE; node = m_moveNodes[node].nextNode)
        {
            m_moveNodes[node].parentNode = parent;
        }
    }
}

int Game::isEqual(const Game& game) const
{
    return ((m_moveNodes == game.m_moveNodes) &&
            (m_annotations == game.m_annotations) &&
            (m_variationStartAnnotations == game.m_variationStartAnnotations));
}

int Game::isBetterOrEqual(const Game& game) const
{
    return ((m_moveNodes.count() >= game.m_moveNodes.count()) &&
            (m_annotations.count() >= game.m_annotations.count()) &&
            (m_variationStartAnnotations.count() >= game.m_variationStartAnnotations.count()));
}
