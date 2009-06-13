/***************************************************************************
                          filename - description
                             -------------------
    begin                :
    copyright            : (C) yyyy [name] <[email]>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtDebug>
#include <QFile>
#include "game.h"

QMap<quint64, QString> Game::m_ecoPositions;

Game::Game()
{
	clear();
	m_isModified = false;
}

Game::Game(const Game& game)
{
	clear();
	//assign non pointer variables
	m_tags = game.m_tags;
	m_startingBoard = game.m_startingBoard;
	m_variationStartAnnotations = game.m_variationStartAnnotations;
	m_annotations = game.m_variationStartAnnotations;

	m_currentNode = game.m_currentNode;
	m_startPly = game.m_startPly;
	m_currentBoard = game.m_currentBoard;

	//copy node array
	m_moveNodes.clear();
	for (int i = 0; i < game.m_moveNodes.size(); ++i) {
		m_moveNodes.append(game.m_moveNodes[i]);
	}
	m_isModified = false;
}

Game& Game::operator=(const Game & game)
{
	clear();
	//assign non pointer variables
	m_tags = game.m_tags;
	m_startingBoard = game.m_startingBoard;
	m_variationStartAnnotations = game.m_variationStartAnnotations;
	m_annotations = game.m_variationStartAnnotations;

	m_currentNode = game.m_currentNode;
	m_startPly = game.m_startPly;
	m_currentBoard = game.m_currentBoard;

	//copy annotations
	m_annotations.clear();
	QMapIterator<int, QString> i(game.m_annotations);
	while (i.hasNext()) {
		i.next();
		m_annotations.insert(i.key(), i.value());
	}
	//copy node array
	m_moveNodes.clear();
	for (int i = 0; i < game.m_moveNodes.size(); ++i) {
		m_moveNodes.append(game.m_moveNodes[i]);
	}
	m_isModified = true;
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
	setAnnotation(annotation);
	m_moveNodes[previousNode].nextNode = m_currentNode;
	m_currentBoard.doMove(move);
	m_isModified = true;

	return m_currentNode;
}

MoveId Game::addMove(const QString& sanMove, const QString& annotation, NagSet nags)
{
	Move move = m_currentBoard.parseMove(sanMove);
	if (move.isLegal())
		return addMove(move, annotation, nags);
	return NO_MOVE;
}

bool Game::replaceMove(const Move& move, const QString& annotation, NagSet nags)
{
	int node;
	node = m_moveNodes[m_currentNode].nextNode;

	if (node == NO_MOVE) {
		addMove(move, annotation, nags);
		return true;
	}

	//replace node data with new move
	m_moveNodes[node].move = move;
	m_moveNodes[node].nags = nags;
	if (!annotation.isEmpty()) {
		setAnnotation(annotation, node);
	}

	//remove any following nodes after replaced move by disconnecting them from the tree
	forward();
	truncateVariation();
	backward();

	m_isModified = true;
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
	MoveId currentNode = m_currentNode;
	moveToId(m_moveNodes[variation].parentNode);
	MoveId demotedVariationParentNode = m_moveNodes[m_currentNode].parentNode;
	MoveId promotedVariationParentNode = m_moveNodes[variation].parentNode;

	while (forward()) {
		m_moveNodes[m_currentNode].parentNode = promotedVariationParentNode;
	}
	moveToId(variation);
	m_moveNodes[m_currentNode].parentNode = demotedVariationParentNode;
	while (forward()) {
		m_moveNodes[m_currentNode].parentNode = demotedVariationParentNode;
	}

	// updating promoted variation parent node
	MoveNode& variationParent = m_moveNodes[promotedVariationParentNode];
	if (variationParent.nextNode != NO_MOVE) {
		QList<MoveId>& vars = variationParent.variations;
		for (int i = 0; i < vars.size(); ++i)
			if (vars.at(i) == variation)
				vars[i] = variationParent.nextNode;
	}
	variationParent.nextNode = variation;

	moveToId(currentNode);
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
	else if (position == BeforeMove) {
		MoveNode firstNode;
		firstNode.nextNode = m_currentNode;
		firstNode.ply = m_moveNodes[m_currentNode].ply - 1;
		m_moveNodes[0] = firstNode;
		m_moveNodes[m_currentNode].previousNode = 0;
		backward();
		m_startingBoard = m_currentBoard;
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
	if (moveId == 0) return true;
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		if ((m_moveNodes[node].previousNode == m_moveNodes[node].parentNode)) {
			return true;
		}
	}
	return false;
}

bool Game::atGameStart(MoveId moveId) const
{
	return (moveId == 0);
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
	//if (int node = nodeValid(moveId)) {
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		if (position == AfterMove) {
			// The default is to add a annotation after the move
			if (annotation.isEmpty())
				m_annotations.remove(node);
			else m_annotations[node] = annotation;
			return true;
		} else {
			// We can also add an annotation before a move
			if (atLineStart(node)) {
				if (annotation.isEmpty())
					m_variationStartAnnotations.remove(node);
				else m_variationStartAnnotations[node] = annotation;
				return true;
			} else {
				// If we are not at the start of a line, the annotation is added
				// after the previous move, assuming that if we are not at the
				// start of a line, there must be a previous move.
				node = m_moveNodes[node].previousNode;
				if (annotation.isEmpty())
					m_annotations.remove(node);
				m_annotations[node] = annotation;
				return true;
			}
		}
	}
	return false;
}

QString Game::annotation(MoveId moveId, Position position) const
{
	MoveId node = nodeValid(moveId);
	if (node != NO_MOVE) {
		if (position == AfterMove) {
			return m_annotations[node];
		} else {
			if (atLineStart(node)) {
				return m_variationStartAnnotations[node];
			} else {
				return m_annotations[m_moveNodes[node].previousNode];
			}
		}
	}
	return QString();
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
		count++;
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
		moved++;
		m_currentBoard.doMove(m_moveNodes[m_currentNode].move);
	}
	return moved;
}

int Game::backward(int count)
{
	int moved = 0;
	while ((m_moveNodes[m_currentNode].previousNode >= 0) && (moved < count)) {
		m_currentBoard.undoMove(m_moveNodes[m_currentNode].move);
		m_currentNode = m_moveNodes[m_currentNode].previousNode;
		moved++;
	}
	return moved;
}

void Game::enterVariation(const MoveId& moveId)
{
	Q_ASSERT(variations().contains(moveId));
	m_currentBoard.doMove(m_moveNodes[moveId].move);
	m_currentNode = moveId;
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
	m_isModified = true;
}

void Game::clearTags()
{
	m_tags.clear();
	m_isModified = true;
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
	clear();
	m_startingBoard = startingBoard;
	m_currentBoard = m_startingBoard;
	m_startPly = (m_startingBoard.moveNumber() - 1) * 2 + (m_startingBoard.toMove() == Black);
}

void Game::setStartingBoard(const QString& fen)
{
	clear();
	m_startingBoard.fromFen(fen);
	m_currentBoard = m_startingBoard;
	m_startPly = (m_startingBoard.moveNumber() - 1) * 2 + (m_startingBoard.toMove() == Black);
}

void Game::setResult(Result result)
{
	m_tags["Result"] = resultString(result);
	m_isModified = true;
}

//QString Game::moveToSan(MoveStringFlags flags, MoveId moveId)
QString Game::moveToSan(MoveStringFlags flags, NextPreviousMove nextPrevious, MoveId moveId)
{
	int saveNode = -1;
	QString san = "";
	MoveId node = nodeValid(moveId);
	Move move;
	if (node != NO_MOVE) {
		if (nextPrevious == PreviousMove) {
			if (node == 0) return san;
			move = m_moveNodes[node].move;
		} else {
			if (m_moveNodes[node].nextNode == NO_MOVE) return san;
			move = m_moveNodes[m_moveNodes[node].nextNode].move;
		}

		if (node != m_currentNode) {
			// save position and jump to node
			saveNode = m_currentNode;
			moveToId(node);
		}
		Board board = m_currentBoard;
		if (nextPrevious == PreviousMove) {
			board.undoMove(move);
		}
		//
		//move number
		if (board.toMove() == Black) {
			if (flags & BlackNumbers) {
				san += QString::number(moveNumber()) + "... ";
			}
		} else {
			if (flags & WhiteNumbers) {
				san += QString::number(moveNumber() + (nextPrevious == NextMove)) + ". ";
			}
		}
		//move
		san += board.moveToSan(move);
		//nags
		if (flags & Nags) {
			QString nagString = nags().toString();
			if (!nags().count() || nagString.startsWith("!") || nagString.startsWith("?")) {
				san += nagString;
			} else {
				san += " " + nagString;
			}
		}
		// Jump back to previous position
		if (saveNode != -1)
			moveToId(saveNode);
	}
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

int Game::findPosition(const BitBoard& position)
{
	moveToStart();

	int current = m_currentNode;
	BitBoard currentBoard(m_currentBoard);

	for (;;) {
		if (currentBoard.positionIsSame(position))
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
