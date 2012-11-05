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

QMap<quint64, QString> Game::m_ecoPositions;

Game::Game()
{
	clear();
    setModified(false);
    m_bValid = true;
}

Game::Game(const Game& game)
{
	clear();
	//assign non pointer variables
	m_tags = game.m_tags;
	m_startingBoard = game.m_startingBoard;
	m_variationStartAnnotations = game.m_variationStartAnnotations;
    m_annotations = game.m_annotations;
    m_squareAnnotations = game.m_squareAnnotations;
    m_arrowAnnotations = game.m_arrowAnnotations;
    m_bValid = game.m_bValid;

	m_currentNode = game.m_currentNode;
	m_startPly = game.m_startPly;
	m_currentBoard = game.m_currentBoard;

	//copy node array
	m_moveNodes.clear();
	for (int i = 0; i < game.m_moveNodes.size(); ++i) {
		m_moveNodes.append(game.m_moveNodes[i]);
	}
    setModified(false);
    m_gameComment = "";

    //copy annotations
    m_annotations.clear();
    QMapIterator<int, QString> i(game.m_annotations);
    while (i.hasNext()) {
        i.next();
        m_annotations.insert(i.key(), i.value());
    }


    //copy square annotations
    m_squareAnnotations.clear();
    QMapIterator<int, QString> j(game.m_squareAnnotations);
    while (j.hasNext()) {
        j.next();
        m_squareAnnotations.insert(j.key(), j.value());
    }

    //copy arrow annotations
    m_arrowAnnotations.clear();
    QMapIterator<int, QString> k(game.m_arrowAnnotations);
    while (k.hasNext()) {
        k.next();
        m_arrowAnnotations.insert(k.key(), k.value());
    }

}

Game& Game::operator=(const Game & game)
{
	if (this!=&game)
	{
		clear();
		//assign non pointer variables
		m_tags = game.m_tags;
		m_startingBoard = game.m_startingBoard;
		m_variationStartAnnotations = game.m_variationStartAnnotations;
        m_annotations = game.m_annotations;
        m_squareAnnotations = game.m_squareAnnotations;
        m_arrowAnnotations = game.m_arrowAnnotations;
        m_bValid = game.m_bValid;

		m_currentNode = game.m_currentNode;
		m_startPly = game.m_startPly;
		m_currentBoard = game.m_currentBoard;
        m_gameComment = game.m_gameComment;

		//copy annotations
		m_annotations.clear();
		QMapIterator<int, QString> i(game.m_annotations);
		while (i.hasNext()) {
			i.next();
			m_annotations.insert(i.key(), i.value());
		}

        //copy square annotations
        m_squareAnnotations.clear();
        QMapIterator<int, QString> j(game.m_squareAnnotations);
        while (j.hasNext()) {
            j.next();
            m_squareAnnotations.insert(j.key(), j.value());
        }

        //copy arrow annotations
        m_arrowAnnotations.clear();
        QMapIterator<int, QString> k(game.m_arrowAnnotations);
        while (k.hasNext()) {
            k.next();
            m_arrowAnnotations.insert(k.key(), k.value());
        }

		//copy node array
		m_moveNodes.clear();
		for (int i = 0; i < game.m_moveNodes.size(); ++i) {
			m_moveNodes.append(game.m_moveNodes[i]);
		}
        setModified(true);
	}
	return *this;
}

Game::~Game()
{
}

MoveId Game::addMove(const Move& move, const QString& annotation, NagSet nags)
{
	MoveNode node;
	MoveId previousNode = m_currentNode;

	node.nextNode = NO_MOVE;
	node.previousNode = m_currentNode;
	node.parentNode = m_moveNodes[m_currentNode].parentNode;
	node.move = move;
	node.nags = nags;
	node.ply = ply() + 1;
	m_moveNodes.append(node);
	m_currentNode = m_moveNodes.size() - 1;
	if (!annotation.isEmpty())
		setAnnotation(annotation);
	m_moveNodes[previousNode].nextNode = m_currentNode;
	m_currentBoard.doMove(move);
    setModified(true);

	return m_currentNode;
}

MoveId Game::addMove(const QString& sanMove, const QString& annotation, NagSet nags)
{
	Move move = m_currentBoard.parseMove(sanMove);
    if (move.isLegal() || move.isNullMove())
		return addMove(move, annotation, nags);
	return NO_MOVE;
}

// does the next main move or one of the variations go from square from to square to
// if so make it on the board
bool Game::findNextMove(Square from, Square to)
{
    int node;
    node = m_moveNodes[m_currentNode].nextNode;
    if( node != NO_MOVE ) {
        Move m = m_moveNodes[node].move ;
        if( m.from() == from && m.to() == to )
        {
            forward();
            return true;
        } else
        {
            QList<MoveId> vs = m_moveNodes[m_currentNode].variations;
            QList<MoveId>::iterator i;
            for (i = vs.begin(); i != vs.end(); ++i)
            {
                Move m = move(*i);
                if( m.from() == from && m.to() == to )
                {
                    moveToId(*i);
                    return true;
                }
            }
         }
    }
    return false;
}

bool Game::replaceMove(const Move& move, const QString& annotation, NagSet nags)
{
	int node;
	node = m_moveNodes[m_currentNode].nextNode;
    setModified(true);
	if (node == NO_MOVE) {
		addMove(move, annotation, nags);
		return true;
	}

	//replace node data with new move
	m_moveNodes[node].move = move;
	m_moveNodes[node].nags = nags;
	setAnnotation(annotation, node);

	//remove any following nodes after replaced move by disconnecting them from the tree
	forward();
	truncateVariation();
	backward();

	return true;
}

bool Game::replaceMove(const QString& sanMove, const QString& annotation, NagSet nags)
{
	return replaceMove(m_currentBoard.parseMove(sanMove), annotation, nags);
}

MoveId Game::addVariation(const Move& move, const QString& annotation, NagSet nags)
{
    MoveId previousNode = m_currentNode;
	MoveId saveNextNode = m_moveNodes[m_currentNode].nextNode;
	MoveId node = addMove(move, annotation, nags);
	m_moveNodes[m_currentNode].parentNode = previousNode;
	m_moveNodes[previousNode].variations.append(node);
	m_moveNodes[previousNode].nextNode = saveNextNode;

	return (m_moveNodes.size() - 1);
}

MoveId Game::addVariation(const MoveList& moveList, const QString& annotation)
{
	if (moveList.isEmpty())
		return NO_MOVE;
	MoveId currentPosition = currentMove();
	MoveId varStart;
	int start = 1;
    if (!atLineEnd()) {
		varStart = addVariation(moveList.first());
	}
	else if (!atGameStart()) {
		Move oldMove = move();
		backward();
		varStart = addVariation(oldMove);
		start = 0;
	}
	else varStart = addMove(moveList.first());
	moveToId(varStart);
    for (int i = start; i < moveList.count(); ++i) {
		addMove(moveList[i]);
		forward();
	}
	if (!annotation.isEmpty())
		setAnnotation(annotation);
	moveToId(currentPosition);
	return varStart;
}

MoveId Game::addVariation(const QString& sanMove, const QString& annotation, NagSet nags)
{
	MoveId previousNode = m_currentNode;
	MoveId saveNextNode = m_moveNodes[m_currentNode].nextNode;
	MoveId node = addMove(sanMove, annotation, nags);
	if (node == NO_MOVE) return node;

	m_moveNodes[m_currentNode].parentNode = previousNode;
	m_moveNodes[previousNode].variations.append(node);
	m_moveNodes[previousNode].nextNode = saveNextNode;

	return (m_moveNodes.size() - 1);
}

bool Game::promoteVariation(MoveId variation)
{
	if (isMainline(variation))
		return false;

	MoveId currentNode = m_currentNode;	// Save current move

	// Find first move of the variation
	while (!atLineStart(variation))
		variation = m_moveNodes[variation].previousNode;
	MoveId parent = m_moveNodes[variation].parentNode;

	// Link old main line to parent 
	reparentVariation(m_moveNodes[parent].nextNode, parent);
	// Link new main line to parent's parent
	reparentVariation(variation, m_moveNodes[parent].parentNode);

	// Swap main line and the variation
	int index = m_moveNodes[parent].variations.indexOf(variation);
	qSwap(m_moveNodes[parent].nextNode, m_moveNodes[parent].variations[index]);

	moveToId(currentNode);	// Restore current move
	return true;
}

bool Game::removeVariation(MoveId variation)
{
	// don't remove whole game
	if (!variation)
		return false;
	MoveId parentNode = m_moveNodes[variation].parentNode;
	removeNode(variation);
	moveToId(parentNode);

	QList<MoveId> &vars = m_moveNodes[m_currentNode].variations;
	vars.removeAt(vars.indexOf(variation));

	compact();
	return true;
}

void Game::truncateVariation(Position position)
{
	if (position == AfterMove) {
		int node = m_moveNodes[m_currentNode].nextNode;
		removeNode(node);
	}
    else if (position == BeforeMove && m_currentNode != 0)
    {
        if (atLineStart())
        {
            backward();
            forward();
        }
		MoveId current = m_currentNode;
		MoveNode firstNode;
		firstNode.nextNode = m_currentNode;
		firstNode.ply = m_moveNodes[m_currentNode].ply - 1;
		// Keep variation if truncating main line
		if (m_moveNodes[m_moveNodes[m_currentNode].previousNode].nextNode == m_currentNode) {
			firstNode.variations = m_moveNodes[m_moveNodes[m_currentNode].previousNode].variations;
			foreach(MoveId var, firstNode.variations) {
				reparentVariation(var, 0);
				m_moveNodes[var].previousNode = 0;
			}
		}
		m_moveNodes[0] = firstNode;
		m_moveNodes[m_currentNode].previousNode = 0;
		backward();
		m_startingBoard = m_currentBoard;
        if (m_startingBoard != standardStartBoard)
        {
            m_tags[TagNames[TagFEN]] = m_startingBoard.toFen();
            m_tags[TagNames[TagSetUp]] = "1";
        }
		moveToId(current);
	}
	compact();
}

const Board& Game::board() const
{
	return m_currentBoard;
}

Board Game::startingBoard() const
{
	return m_startingBoard;
}

QString Game::toFen() const
{
	return m_currentBoard.toFen();
}

QString Game::gameComment() const
{
    return m_gameComment;
}

void Game::setGameComment(const QString& gameComment)
{
    m_gameComment = gameComment;
}


bool Game::isMainline(MoveId moveId) const
{
	if (moveId == 0) return true;
	MoveId node = nodeValid(moveId);
	if (node == NO_MOVE)
		return false;
	else return m_moveNodes[node].parentNode == NO_MOVE;
}

Result Game::result() const
{
	if (m_tags["Result"] == "1-0") {
		return WhiteWin;
	} else if (m_tags["Result"] == "1/2-1/2") {
		return Draw;
	} else if (m_tags["Result"] == "0-1") {
		return BlackWin;
	} else {
		return Unknown;
	}
}

bool Game::atLineStart(MoveId moveId) const
{
	MoveId node = nodeValid(moveId);
	if (node == NO_MOVE) 
		return false;
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
	if (node != NO_MOVE) {
		if (m_moveNodes[node].nextNode == NO_MOVE) {
			return true;
		}
	}

	return false;
}

bool Game::setAnnotation(QString annotation, MoveId moveId, Position position)
{

    MoveId node = nodeValid(moveId);
    if (node == NO_MOVE)
        return false;

    QString sqAnnot = specialAnnotation(annotation, "[%csl");
    if( position == Game::AfterMove || node == 0 )
    {
        setSquareAnnotation(sqAnnot,node);
    }else
    {
        setSquareAnnotation(sqAnnot,m_moveNodes[node].previousNode);
    }

    QString arrowAnnot = specialAnnotation(annotation, "[%cal");
    if( position == Game::AfterMove || node == 0)
    {
        setArrowAnnotation(arrowAnnot,node);
    }else
    {
        setArrowAnnotation(arrowAnnot,m_moveNodes[node].previousNode);
    }

    int moves;
    int comments;
    int nags;
    moveCount(&moves,&comments,&nags);
    if( moves <= 0 )
    {
        setGameComment(annotation);
        return true;
    }

	if (position == AfterMove) {
		if (annotation.isEmpty())
			m_annotations.remove(node);
		else m_annotations[node] = annotation;
	} 
	else if (canHaveStartAnnotation(node)) {	// Pre-move comment
		if (annotation.isEmpty())
			m_variationStartAnnotations.remove(node);
		else m_variationStartAnnotations[node] = annotation;
	}
	return true;
}

bool Game::setSquareAnnotation(QString squareAnnotation, MoveId node)
{

        if (squareAnnotation.isEmpty())
            m_squareAnnotations.remove(node);
        else m_squareAnnotations[node] = squareAnnotation;
    return true;
}

QString Game::squareAnnotation(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node == NO_MOVE)
        return "";

    QString annotation = m_squareAnnotations[node];
    if(annotation.isNull()) {
        return "";
    }
    return m_squareAnnotations[node];
}

bool Game::setArrowAnnotation(QString arrowAnnotation, MoveId node)
{
        if (arrowAnnotation.isEmpty())
            m_arrowAnnotations.remove(node);
        else m_arrowAnnotations[node] = arrowAnnotation;
    return true;
}

QString Game::arrowAnnotation(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node == NO_MOVE)
        return "";

    QString annotation = m_arrowAnnotations[node];
    if(annotation.isNull()) {
        return "";
    }
    return m_arrowAnnotations[node];
}


QString Game::annotation(MoveId moveId, Position position) const
{
	MoveId node = nodeValid(moveId);
	if (position == AfterMove)
		return m_annotations.value(node, "");
	else 
		return m_variationStartAnnotations.value(node, "");
}

bool Game::canHaveStartAnnotation(MoveId moveId) const
{
	MoveId node = nodeValid(moveId);
	return atLineStart(moveId) || atGameStart(m_moveNodes[node].previousNode);
}

bool Game::addNag(Nag nag, MoveId moveId)
{
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		m_moveNodes[node].nags.addNag(nag);
		return true;
	}
	return false;
}

bool Game::setNags(NagSet nags, MoveId moveId)
{
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		m_moveNodes[node].nags = nags;
		return true;
	}
	return false;
}

NagSet Game::nags(MoveId moveId) const
{
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		return m_moveNodes[node].nags;
	}
	return NagSet();
}

MoveId Game::nodeValid(MoveId moveId) const
{
	if (moveId == CURRENT_MOVE) {
		moveId = m_currentNode;
	}
	if ((moveId >= 0) && (moveId < m_moveNodes.size())) {
		if (m_moveNodes[moveId].removed) {
			return NO_MOVE;
		}
		return moveId;
	}
	return NO_MOVE;
}

void Game::moveCount(int* moves, int* comments, int* nags)
{
	*moves = *comments = *nags = 0;

	int node = 1;
	while (nodeValid(node) != NO_MOVE) {
		*moves += 1;
		if (m_moveNodes[node].nags.count() != 0) {
			*nags += 1;
		}
		node = m_moveNodes[node].nextNode;
	}
	// Count comments
	for (int i = 0; i < m_annotations.size(); ++i) {
		*comments += 1;
	}
	for (int i = 0; i < m_variationStartAnnotations.size(); ++i) {
		*comments += 1;
	}
}

int Game::ply(MoveId moveId) const
{
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		return m_moveNodes[node].ply;
	}
	return 0;
}

int Game::moveNumber(MoveId moveId) const
{
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		if (int plyNum = ply(node))
			return (m_startPly + plyNum - 1) / 2 + 1;
		return 0;
	}
	return -1;
}

int Game::plyCount() const
{
	int count = 0;
	int node = 0;

	while (node != NO_MOVE) {
        ++count;
		node = m_moveNodes[node].nextNode;
	}
	// Counted one too much, because we have to start at zero
	// (an empty game has no node 1)
	return count - 1;
}

MoveId Game::variationNumber(MoveId moveId) const
{
	if (isMainline()) return 0;
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		int parentNode = m_moveNodes[node].parentNode;
		for (int i = 0; i < m_moveNodes.size(); ++i) {
			if ((m_moveNodes[i].previousNode == parentNode) &&
					(m_moveNodes[i].parentNode == parentNode)) {
				return i;
			}
		}
	}
	return 0;
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

const QList<MoveId>& Game::variations() const
{
	return m_moveNodes[m_currentNode].variations;
}

int Game::variationCount(MoveId moveId) const
{
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		return m_moveNodes[node].variations.size();
	}
	return 0;
}

bool Game::isModified() const
{
	return m_isModified;
}

void Game::setModified(bool set)
{
	m_isModified = set;
}

void Game::moveToStart()
{
	m_currentNode = 0;
	m_currentBoard = m_startingBoard;

    QString annotation = squareAnnotation(m_currentNode);
    m_currentBoard.setSquareAnnotation( annotation);

    annotation = arrowAnnotation(m_currentNode);
    m_currentBoard.setArrowAnnotation( annotation);

}

int Game::moveByPly(int diff)
{
	if (diff > 0) {
		return forward(diff);
	} else {
		return backward(-diff);
	}
}

void Game::moveToId(MoveId moveId)
{
	if (nodeValid(moveId) == NO_MOVE)
		return;

	//jump to node, travelling back to start adding the moves to the stack
	int node = moveId;
	QStack < Move > moveStack;
	while (node) {
		moveStack.push(m_moveNodes[node].move);
		node = m_moveNodes[node].previousNode;
	}

	//reset the board, then make the moves on the stack to create the correct position
	m_currentNode = moveId;
	m_currentBoard = m_startingBoard;
	while (!moveStack.isEmpty()) {
		m_currentBoard.doMove(moveStack.pop());
	}
    QString sqannotation = squareAnnotation(moveId);
    m_currentBoard.setSquareAnnotation( sqannotation);

    QString aannotation = arrowAnnotation(moveId);
    m_currentBoard.setArrowAnnotation( aannotation);

}

int Game::moveToPly(int ply)
{
	if (ply == 0) {
		moveToStart();
		return 0;
	}

	int currentPly = this->ply();
	int diff = ply - currentPly;

	if (diff > 0) {
		return currentPly + forward(diff);
	} else {
		return currentPly - backward(-diff);
	}
}

Move Game::move(MoveId moveId) const
{
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		return m_moveNodes[node].move;
	}
	return Move();
}

void Game::moveToEnd()
{
	// Move out of variations to mainline
	while (m_moveNodes[m_currentNode].parentNode != NO_MOVE) {
		moveToId(m_moveNodes[m_currentNode].parentNode);
	}
	// Now move forward to the end of the game
	while (m_moveNodes[m_currentNode].nextNode != NO_MOVE) {
		forward(1);
    }
}

int Game::forward(int count)
{
	int moved = 0;
	while ((m_moveNodes[m_currentNode].nextNode != NO_MOVE) && (moved < count)) {
		m_currentNode = m_moveNodes[m_currentNode].nextNode;
        ++moved;

		m_currentBoard.doMove(m_moveNodes[m_currentNode].move);
	}
    QString annotation = squareAnnotation(m_currentNode);
    m_currentBoard.setSquareAnnotation( annotation);

    annotation = arrowAnnotation(m_currentNode);
    m_currentBoard.setArrowAnnotation( annotation);

	return moved;
}

int Game::backward(int count)
{
	int moved = 0;
	while ((m_moveNodes[m_currentNode].previousNode >= 0) && (moved < count)) {
		m_currentBoard.undoMove(m_moveNodes[m_currentNode].move);
		m_currentNode = m_moveNodes[m_currentNode].previousNode;
        ++moved;
	}

    QString annotation = squareAnnotation(m_currentNode);
    m_currentBoard.setSquareAnnotation( annotation);

    annotation = arrowAnnotation(m_currentNode);
    m_currentBoard.setArrowAnnotation( annotation);

	return moved;
}

void Game::enterVariation(const MoveId& moveId)
{
	Q_ASSERT(variations().contains(moveId));
	m_currentBoard.doMove(m_moveNodes[moveId].move);
	m_currentNode = moveId;

    QString annotation = squareAnnotation(m_currentNode);
    m_currentBoard.setSquareAnnotation(annotation);

    annotation = arrowAnnotation(m_currentNode);
    m_currentBoard.setArrowAnnotation(annotation);

}

void Game::removeNode(MoveId moveId)
{
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		if (variationCount(node)) {
			for (int i = 0; i < m_moveNodes[node].variations.size(); ++i) {
				removeNode(m_moveNodes[node].variations[i]);
			}
		}
		removeNode(m_moveNodes[node].nextNode);
		if (!atLineStart(node)) {
			m_moveNodes[m_moveNodes[node].previousNode].nextNode = NO_MOVE;
		}
		m_moveNodes[node].remove();
	}
}

void Game::clear()
{
	clearTags();
	m_moveNodes.clear();
	m_variationStartAnnotations.clear();
	m_annotations.clear();
	m_startPly = 0;
	m_currentNode = 0;

	m_startingBoard.setStandardPosition();
	m_currentBoard = m_startingBoard;

	m_moveNodes.append(MoveNode());
    setModified(true);
}

void Game::clearTags()
{
	m_tags.clear();
    setModified(true);
}

QString Game::tag(const QString& tag) const
{
	return m_tags[tag];
}

QMap<QString, QString> Game::tags() const
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

void Game::setStartingBoard(const Board& startingBoard)
{
    setStartingBoard(startingBoard.toFen());
}

void Game::setStartingBoard(const QString& fen)
{
    clear();
	m_startingBoard.fromFen(fen);
    if (m_startingBoard != standardStartBoard)
    {
        m_tags[TagNames[TagFEN]] = fen;
        m_tags[TagNames[TagSetUp]] = "1";
    }
	m_currentBoard = m_startingBoard;
	m_startPly = (m_startingBoard.moveNumber() - 1) * 2 + (m_startingBoard.toMove() == Black);
}

void Game::setResult(Result result)
{
	m_tags["Result"] = resultString(result);
    setModified(true);
}

QString Game::moveToSan(MoveStringFlags flags, NextPreviousMove nextPrevious, MoveId moveId)
{
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE && nextPrevious == NextMove)
		node = m_moveNodes[node].nextNode;
	if (node == NO_MOVE)
		return QString();

	MoveNode move;
	move = m_moveNodes[node];
    if (!( move.move.isLegal() || move.move.isNullMove()))
		return QString();

	// Save current node
	MoveId saveNode = NO_MOVE;
	MoveId boardNode = m_moveNodes[node].previousNode;
	if (boardNode != m_currentNode) {
		saveNode = m_currentNode;
		moveToId(boardNode);
	}

	// Move number
	QString san;
	if (m_currentBoard.toMove() == Black && flags & BlackNumbers) 
		san += QString::number(moveNumber(node)) + "... ";
	else if (m_currentBoard.toMove() == White && flags & WhiteNumbers) 
		san += QString::number(moveNumber(node)) + ". ";

	// Move and SAN
	san += m_currentBoard.moveToSan(move.move);
	if (flags & Nags)
		san += nags(node).toString();
		
	// Restore previous position
	if (saveNode != NO_MOVE)
		moveToId(saveNode);
	
	return san;
}

void Game::dumpMoveNode(MoveId moveId)
{
	if (moveId == CURRENT_MOVE) {
		moveId = m_currentNode;
	}
	if (moveId != NO_MOVE) {
		qDebug() << "Move Id : " << moveId;
		qDebug() << "   Next node   : " << m_moveNodes[moveId].nextNode;
		qDebug() << "   Prev node   : " << m_moveNodes[moveId].previousNode;
		qDebug() << "   Parent node : " << m_moveNodes[moveId].parentNode;
		qDebug() << "   Nags        : " << m_moveNodes[moveId].nags.toString();
		qDebug() << "   Deleted     : " << m_moveNodes[moveId].removed;
		qDebug() << "   # Variations: " << m_moveNodes[moveId].variations.size();
		qDebug() << "   Variations  : " << m_moveNodes[moveId].variations;
		qDebug() << "   Move        : " << moveToSan(FullDetail, PreviousMove, moveId);
	}
}

void Game::dumpAllMoveNodes()
{
	qDebug() << "Current Node: " << m_currentNode;
	for (int i = 0; i < m_moveNodes.size(); ++i) {
		dumpMoveNode(i);
		if (!m_annotations.value(i).isEmpty()) {
			qDebug() << "   Annotation : " << m_annotations.value(i);
		}
	}
	int moves, comments, nags;
	moveCount(&moves, &comments, &nags);
	qDebug() << "Moves: " << moves << " Comments: " << comments << " Nags: " << nags;
}

int Game::findPosition(const Board& position)
{
	moveToStart();

	int current = m_currentNode;
	Board currentBoard(m_currentBoard);

	for (;;) {
		if (currentBoard == position && currentBoard.positionIsSame(position))
			return current;

		current = m_moveNodes[current].nextNode;
		if (current == NO_MOVE || !position.canBeReachedFrom(currentBoard))
			return NO_MOVE;

		currentBoard.doMove(m_moveNodes[current].move);
	}
	return NO_MOVE;
}

bool Game::loadEcoFile(const QString& ecoFile)
{
	QFile file(ecoFile);
	file.open(QIODevice::ReadOnly);
	QDataStream sin(&file);
	quint32 id;
	sin >> id;
	sin >> m_ecoPositions;
	file.close();
	if (id != COMPILED_ECO_FILE_ID)
		return false;
	return true;
}

void Game::compact()
{
	int oldSize = m_moveNodes.size();
	QList <MoveNode> moveNodes;
	QVector <int> oldIdNewIdMapping(oldSize, NO_MOVE);

	for (int i = 0; i < oldSize; ++i) {
		if (!m_moveNodes[i].removed) {
			oldIdNewIdMapping[i] = moveNodes.size();
			moveNodes.append(m_moveNodes[i]);
		}
	}

	// update nodes links to other nodes in shrinked list (prev, next, variations)
	for (int i = 0, newSize = moveNodes.size(); i < newSize; ++i) {
		MoveNode& node = moveNodes[i];
#define GAME_UPDATE_MOVEID(aMoveId) if (aMoveId != NO_MOVE) aMoveId = oldIdNewIdMapping[aMoveId]
		GAME_UPDATE_MOVEID(node.nextNode);
		GAME_UPDATE_MOVEID(node.previousNode);
		GAME_UPDATE_MOVEID(node.parentNode);
		QList<MoveId>& vars = node.variations;
		for (int j = 0; j < vars.size(); ++j)
			GAME_UPDATE_MOVEID(vars[j]);
		vars.removeAll(NO_MOVE);
#undef  GAME_UPDATE_LINK
	}

	m_moveNodes.clear();
	m_moveNodes = moveNodes;
	m_currentNode = oldIdNewIdMapping[m_currentNode];
}

QString Game::ecoClassify()
{
	//move to end of main line
	MoveId currentNode = m_currentNode;
	moveToEnd();

	//search backwards for the first eco position
	while (backward()) {
		quint64 key = m_currentBoard.getHashValue();
		if (m_ecoPositions.contains(key)) {
			moveToId(currentNode);
			return m_ecoPositions[key];
		}
	}
	moveToId(currentNode);

	return QString();
}


void Game::reparentVariation(MoveId variation, MoveId parent)
{
	for (MoveId node = variation; node != NO_MOVE; node = m_moveNodes[node].nextNode)
		m_moveNodes[node].parentNode = parent;
}

QString Game::specialAnnotation(QString& annotation, QString specialMark ) // [%csl  [%cal
{
    QString result = "";
    int specialAnnotationStart = annotation.indexOf(specialMark);
    // If we found a chessbase special annotation
    if(specialAnnotationStart >= 0 ) {

        int specialAnnotationDataStart = specialAnnotationStart + specialMark.length() + 1; //+ 1 for space after specialMark
        int specialAnnotationEnd = annotation.indexOf(']', specialAnnotationDataStart);

        if(specialAnnotationEnd > specialAnnotationDataStart )
        {
            result = annotation.mid(specialAnnotationDataStart,specialAnnotationEnd - specialAnnotationDataStart );
        };
    }
    return result;
}

void Game::setValid(bool valid)
{
    m_bValid = valid;
}

bool Game::isValid() const
{
    return m_bValid;
}

