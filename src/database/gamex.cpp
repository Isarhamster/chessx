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
#include <QRegularExpression>
#include "annotation.h"
#include "ecopositions.h"
#include "gamex.h"
#include "settings.h"
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

const QList<QRegularExpression> GameX::s_specList = QList<QRegularExpression>() << SquareAnnotation().filter() << ArrowAnnotation().filter() << EvalAnnotation().filter();

GameX::GameX()
    : QObject()
    , m_moves()
    , m_variationStartAnnotations()
    , m_annotations()
    , m_nags()
    , m_tags()
    , m_needsCleanup(false)
{
}

GameX::GameX(const GameX& game)
    : QObject()
    , m_moves(game.m_moves)
    , m_variationStartAnnotations(game.m_variationStartAnnotations)
    , m_annotations(game.m_annotations)
    , m_nags(game.m_nags)
    , m_tags(game.m_tags)
    , m_needsCleanup(game.m_needsCleanup)
{
    if (m_moves.currentBoard() && !game.m_moves.currentBoard())
    {
        moveToStart();
    }
}

GameX& GameX::operator=(const GameX& game)
{
    if (this != &game)
    {
        m_moves = game.m_moves;
        m_variationStartAnnotations = game.m_variationStartAnnotations;
        m_annotations = game.m_annotations;
        m_nags = game.m_nags;
        m_tags = game.m_tags;
        m_needsCleanup = game.m_needsCleanup;
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
        emt = ElapsedMoveTimeAnnotation(QString("0:%1:%2").arg(tl[0],-2,'0').arg(tl[1],-2,'0')).asAnnotation();
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
        if(NO_MOVE == (otherMergeNode = otherGame.cursor().findPosition(board())))
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
        shortDescription = QString("%1-%2 %3").arg(white, black, event);
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
        if (board().compare(b))
        {
            repCount++;
            if (repCount >= 3) break;
        }
        Move m = m_moves.move();
        if (m.isCapture() || m.isCastling())
        {
            break;
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
    return m_moves.variations();
}

bool GameX::currentNodeHasMove(Square from, Square  to) const
{
    if(currentNodeHasVariation(from, to))
    {
        return true;
    }
    MoveId node = m_moves.nextMove();
    if(node == NO_MOVE)
    {
        return false;
    }
    Move m = m_moves.move(node);
    return (m.from() == from && m.to() == to);
}

bool GameX::hasNextMove() const
{
    auto next = m_moves.nextMove();
    return (next != NO_MOVE);
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
    node = m_moves.nextMove();
    if(node != NO_MOVE)
    {
        Move m = m_moves.move(node);
        if(m.from() == from && m.to() == to &&
                ((promotionPiece == None) || ((m.isPromotion() && (pieceType(m.promotedPiece()) == promotionPiece)))))
        {
            forward();
            return true;
        }
        else
        {
            QList<MoveId> vs = m_moves.variations();
            QList<MoveId>::iterator i;
            for(i = vs.begin(); i != vs.end(); ++i)
            {
                Move m = move(*i);
                if(m.from() == from && m.to() == to &&
                        ((promotionPiece == None) || ((m.isPromotion() && (pieceType(m.promotedPiece()) == promotionPiece)))))
                {
                    enterVariation(*i);
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

    node = m_moves.nextMove();
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

void GameX::clearDummyNodes()
{
    m_moves.clearDummyNodes();
    compact();
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

void GameX::dbPromoteVariation(MoveId variation)
{
    if(!isMainline(variation))
    {
        m_moves.promoteVariation(variation);
    }
}

void GameX::promoteVariation(MoveId variation)
{
    if(!isMainline(variation))
    {
        GameX state = *this;
        dbPromoteVariation(variation);
        emit signalGameModified(true, state, tr("Promote variation"));
    }
}

bool GameX::dbRemoveVariation(MoveId variation)
{
    bool success = m_moves.removeVariation(variation);
    if (success)
    {
        compact();
    }
    return success;
}

bool GameX::removeVariation(MoveId variation)
{
    GameX state = *this;
    bool success = dbRemoveVariation(variation);
    if (success)
    {
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
        MoveId node = m_moves.nextMove();
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

void GameX::removeNullLinesDb()
{
    moveToStart();
    m_moves.removeNullLines();
    compact();
}

void GameX::removeVariations()
{
    GameX state = *this;
    removeVariationsDb();
    emit signalGameModified(true, state, tr("Remove variations"));
}

void GameX::removeNullLines()
{
    GameX state = *this;
    removeNullLinesDb();
    emit signalGameModified(true, state, tr("Remove null lines"));
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
    QRegularExpression tan = TimeAnnotation().filter();
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

bool GameX::prependAnnotation(QString a, char delimiter, MoveId moveId, Position position)
{
    GameX state = *this;
    if (dbPrependAnnotation(a, delimiter, moveId, position))
    {
        emit signalGameModified(false, state, QString());
        return true;
    }
    return false;
}

bool GameX::editAnnotation(QString annotation, MoveId moveId, Position position)
{
    GameX state = *this;
    QString cleanAnnotation = GameX::cleanAnnotation(annotation, GameX::FilterAll);
    QString spec = specAnnotations(moveId);
    annotation = cleanAnnotation.prepend(spec);

    if (dbSetAnnotation(annotation, moveId, position))
    {
        dbIndicateAnnotationsOnBoard();
        emit signalGameModified(true, state, "Edit annotation");
        return true;
    }
    return false;
}

bool GameX::appendAnnotation(QString annotation, MoveId moveId, Position position)
{
    GameX state = *this;
    if (dbAppendAnnotation(annotation, moveId, position))
    {
        dbIndicateAnnotationsOnBoard();
        emit signalGameModified(true, state, "Append annotation");
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

bool GameX::dbPrependAnnotation(QString a, char delimiter, MoveId moveId, Position position)
{
    QString s = annotation();
    if (s.length())
    {
        a.append(delimiter);
    }
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
            if(annotation.isEmpty() && (node > 1)) // Do not remove empty precomment before move 1 as this would convert game comment to pre comment
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

bool GameX::appendSquareAnnotation(Square s, QChar colorCode)
{
    GameX state = *this;
    QString sq = strSquareNames[s];

    SquareAnnotation squareAnnot(squareAnnotation());

    if(colorCode != QChar(0))
    {
        squareAnnot.removeOne(QRegularExpression(QString("[^%1]").arg(colorCode) + sq));
        squareAnnot.toggle(QString("%1%2").arg(colorCode).arg(sq));
    }
    else
    {
        squareAnnot.removeOne(QRegularExpression(QString(".") + sq));
    }

    setSpecAnnotation(squareAnnot);

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
    QString sqSrc = strSquareNames[src];
    QString sqDest = strSquareNames[dest];

    ArrowAnnotation arrow(arrowAnnotation());

    if(colorCode != QChar(0))
    {
        arrow.removeOne(QRegularExpression(QString("[^%1]").arg(colorCode) + sqSrc + sqDest));
        arrow.toggle(QString("%1%2%3").arg(colorCode).arg(sqSrc, sqDest));
    }
    else
    {
        arrow.removeOne(QRegularExpression(QString(".") + sqSrc + sqDest));
    }

    setSpecAnnotation(arrow);

    emit signalGameModified(true, state, tr("Paint arrow"));
    return true;
}

QString GameX::squareAnnotation(MoveId moveId) const
{
    QString s = specAnnotation(SquareAnnotation().filter(), moveId);
    return s;
}

bool GameX::setSpecAnnotation(const Annotation& a)
{
    QString s = annotation();
    s.remove(a.filter());

    if(!a.isEmpty())
    {
        s.append(a.asAnnotation());
    }
    dbSetAnnotation(s);
    indicateAnnotationsOnBoard();
    return true;
}

QString GameX::arrowAnnotation(MoveId moveId) const
{
    QString s = specAnnotation(ArrowAnnotation().filter(), moveId);
    return s;
}

QString GameX::specAnnotation(const QRegularExpression &r, MoveId moveId) const
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

    QRegularExpressionMatch match;
    int pos = annotation.indexOf(r, 0, &match);

    if(pos >= 0)
    {
        return match.captured(2);
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
            moveId = m_moves.prevMove(moveId);
        }
        else return "";
    }

    QString s = specAnnotation(TimeAnnotation().filter(), moveId);
    s = s.trimmed();
    return s;
}

void GameX::setTimeAnnotation(QString a, MoveId moveId)
{
    MoveId node = m_moves.makeNodeIndex(moveId);
    if (node>ROOT_NODE)
    {
        QString s = annotation(node);
        s.remove(TimeAnnotation().filter());
        dbSetAnnotation(s, node);
        appendAnnotation(a, node);
    }
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

QString GameX::specAnnotations(QString s) const
{
    QString retval;
    foreach (QRegularExpression r, s_specList)
    {
        QRegularExpressionMatch match;
        if (s.indexOf(r,0,&match)>=0)
        {
            retval += match.captured(0);
        }
    }
    return retval;
}

QString GameX::specAnnotations(MoveId moveId, Position position) const
{
    QString s = annotation(moveId, position);
    return specAnnotations(s);
}

/* static */ QString GameX::cleanAnnotation(QString s, AnnotationFilter f)
{
    if (!s.isEmpty())
    {
        if (f&FilterTan)  s.remove(TimeAnnotation().filter());
        if (f&FilterCan)  s.remove(VisualAnnotation().filter());
        if (f&FilterEval) s.remove(EvalAnnotation().filter());
    }
    return s;
}

GameX::AnnotationFilter GameX::textFilter() const
{
    return AppSettings->getValue("/GameText/HideSpecAnnotations").toBool() ? FilterAll : FilterNone;
}

GameX::AnnotationFilter GameX::textFilter2() const
{
    return AppSettings->getValue("/GameText/HideSpecAnnotations").toBool() ? static_cast<GameX::AnnotationFilter>(GameX::FilterTan | GameX::FilterCan) : FilterNone;
}

QString GameX::textAnnotation(MoveId moveId, Position position, AnnotationFilter f) const
{
    QString s = annotation(moveId, position);
    return cleanAnnotation(s, f);
}

bool GameX::canHaveStartAnnotation(MoveId moveId) const
{
    MoveId node = m_moves.makeNodeIndex(moveId);
    return atLineStart(moveId) || atGameStart(m_moves.prevMove(node));
}

void GameX::dbAddNag(Nag nag, MoveId moveId)
{
    if (nag != NullNag)
    {
        MoveId node = m_moves.makeNodeIndex(moveId);
        if (node != NO_MOVE)
        {
            m_nags[node].addNag(nag);
        }
    }
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
        for (const auto& value: m_nags)
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
        MoveId parentNode = m_moves.parentMove(node);
        const auto& v = m_moves.variations(parentNode);
        if (!v.empty())
        {
            for(int i = 0; i < v.size(); ++i)
            {
                QString oldAnnotation = annotation(v[i], GameX::BeforeMove);
                oldAnnotation.remove(QRegularExpression("^.\\)"));
                QString s = QString("%1) %2").arg(QChar(a + i)).arg(oldAnnotation).trimmed();
                dbSetAnnotation(s, v[i], GameX::BeforeMove);
            }
            emit signalGameModified(true, state, tr("Enumerate variations"));
        }
    }
}

MoveId GameX::lastMove() const
{
    MoveId moveId = ROOT_NODE;
    MoveId nextId = m_moves.nextMove(moveId);
    while (nextId != NO_MOVE)
    {
        moveId = nextId;
        nextId = m_moves.nextMove(moveId);
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

void GameX::indicateAnnotationsOnBoard()
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
    if (!m_moves.moveToLineEnd())
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
    for (auto node: qAsConst(removed))
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

void GameX::setSourceTag(const QString& value)
{
    if(AppSettings->getValue("/General/mergeAddSource").toBool())
    {
        setTag(AppSettings->getValue("/General/mergeAddTag").toString(), value);
    }
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
    m_moves.clear(fen, chess960);
    m_variationStartAnnotations.clear();
    m_annotations.clear();
    m_nags.clear();
    dbSetChess960(chess960);
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
        node = m_moves.nextMove(node);
    }
    if(node == NO_MOVE)
    {
        return QString();
    }

    const auto& move = m_moves.moveAt(node);
    if(!(move.isLegal() || move.isNullMove()))
    {
        return QString();
    }

    // Save current node
    MoveId saveNode = NO_MOVE;
    MoveId boardNode = m_moves.prevMove(node);
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
    san += m_moves.currentBoard()->moveToSan(move, flags & TranslatePiece);
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
        qDebug() << "     Nags        : " << m_nags.value(moveId, NagSet()).toString(NagSet::Simple);
    }
}

void GameX::dumpAllMoveNodes() const
{
    qDebug() << Qt::endl;
    qDebug() << "Current Node: " << m_moves.currMove();
    for(int i = 0, sz = m_moves.capacity(); i < sz; ++i)
    {
        m_moves.dumpMoveNode(i);
        dumpAnnotations(i);
    }
    int moves, comments, nags;
    moveCount(&moves, &comments, &nags);
    qDebug() << "Moves: " << moves << " Comments: " << comments << " Nags: " << nags << Qt::endl;
    qDebug() << "----------------------------------" << Qt::endl;
}

void GameX::compact()
{
//    MoveId currentNode = currentMove();
//    if (m_moves.isRemoved(currentNode))
//    {
//        // All kind of funny stuff happens once the current node is deleted
//        m_moves.moveToStart();
//    }
    auto renames = m_moves.compact();
    applyRenames(m_annotations, renames);
    applyRenames(m_variationStartAnnotations, renames);
    applyRenames(m_nags, renames);
}

bool GameX::needsCleanup() const
{
    return m_needsCleanup;
}

void GameX::setNeedsCleanup(bool value)
{
    m_needsCleanup = value;
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

    // Because we now only move backward() after the loop, we must ensure there are some moves
    if (g.m_moves.countMoves() == 0) {
        return QString();
    }

    // Search backwards for the first eco position
    do
    {
        QString eco;
        if (EcoPositions::isEcoPosition(g.board(),eco))
        {
            return eco;
        }
    } while(g.m_moves.backward());

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

    do
    {
        int score = g.board().score();
        scores.append(score);
    } while(g.forward());
}

void GameX::evaluation(double& d) const
{
    QRegularExpression eval = EvalAnnotation().filter();
    QRegularExpressionMatch match;
    int pos = annotation().indexOf(eval, 0, &match);
    if(pos >= 0)
    {
        QString w = match.captured(2);
        bool ok;
        double f = w.toDouble(&ok);
        if (ok)
        {
            d = f;
        }
    }
}

void GameX::scoreEvaluations(QList<double>& evaluations) const
{
    GameX g = *this;
    g.moveToStart();
    evaluations.clear();
    double score = 0.0;
    g.evaluation(score);
    evaluations.append(score);
    while(g.forward())
    {
        g.evaluation(score);
        evaluations.append(score);
    }
}

int GameX::isEqual(const GameX& game) const
{
    return ((m_moves.isEqual(game.m_moves)) &&
            (m_nags == game.m_nags) &&
            (m_annotations == game.m_annotations) &&
            (m_variationStartAnnotations == game.m_variationStartAnnotations));
}

int GameX::isBetterOrEqual(const GameX& game) const
{
    return ((m_moves.capacity() >= game.m_moves.capacity()) &&
            (m_annotations.count() >= game.m_annotations.count()) &&
            (m_variationStartAnnotations.count() >= game.m_variationStartAnnotations.count()));
}
