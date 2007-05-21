/*************************************nodeCount**************************************
                          game.cpp - chess game
                             -------------------
    begin                : 27 August 2005
    copyright            : (C) 2005, 2006 William Hoggarth
														<whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QFile>
#include <QRegExp>
#include <QStringList>
#include <QTextStream>
#include <QStack>

#include "game.h"

QMap<quint64, QString> Game::m_ecoPositions;

Game::Game()
{
	m_startBoard.fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	m_currentBoard = m_startBoard;
	m_startAnnotation = QString();
	m_result = Unknown;

	m_currentNode = 0;
	m_ply = 0;
	m_startPly = 0;
	m_nextFreeNode = 1;
	m_deletedNodeCount = 0;
	m_totalNodeCount = defaultSize;
	m_moveNodes = new MoveNode[defaultSize];
	m_moveNodes[0].nextNode = 0;
	m_moveNodes[0].parentNode = 0;
	m_moveNodes[0].nextVariation = 0;
}

Game::Game(const Game& game)
{
	//assign non pointer variables
	m_tags = game.m_tags;

	m_startBoard = game.m_startBoard;
	m_startAnnotation = game.m_startAnnotation;

	m_result = game.m_result;

	m_currentNode = game.m_currentNode;
	m_ply = game.m_ply;
	m_startPly = game.m_startPly;
	m_currentBoard = game.m_currentBoard;
	m_history = game.m_history;

	m_nextFreeNode = game.m_nextFreeNode;
	m_deletedNodeCount = game.m_deletedNodeCount;
	m_totalNodeCount = game.m_totalNodeCount;

	//copy node array
	m_moveNodes = new MoveNode[m_totalNodeCount];
	memcpy(m_moveNodes, game.m_moveNodes, sizeof(MoveNode) * m_totalNodeCount);
}

Game& Game::operator=(const Game& game)
{
	//assign non pointer variables
	m_tags = game.m_tags;

	m_startBoard = game.m_startBoard;
	m_startAnnotation = game.m_startAnnotation;
	m_result = game.m_result;

	m_currentNode = game.m_currentNode;
	m_ply = game.m_ply;
	m_startPly = game.m_startPly;
	m_currentBoard = game.m_currentBoard;
	m_history = game.m_history;

	m_nextFreeNode = game.m_nextFreeNode;
	m_deletedNodeCount = game.m_deletedNodeCount;

	//copy node array
	if (m_totalNodeCount < game.m_nextFreeNode) {
		m_totalNodeCount = game.m_totalNodeCount;
		MoveNode* moveNodes = new MoveNode[m_totalNodeCount];
		for (int node = 0; node < game.m_nextFreeNode; node++) {
			moveNodes[node] = game.m_moveNodes[node];
		}
		delete[] m_moveNodes;
		m_moveNodes = moveNodes;
	} else {
		for (int node = 0; node < game.m_nextFreeNode; node++) {
			m_moveNodes[node] = game.m_moveNodes[node];
		}
	}

	return *this;
}

Game::~Game()
{
	delete[] m_moveNodes;
}

Board Game::board() const
{
	return m_currentBoard;
}

QString Game::toFen() const
{
	return m_currentBoard.toFEN(moveNumber());
}

bool Game::isMainline()
{
	return !m_moveNodes[m_currentNode].parentNode;
}

bool Game::isMoveLegal(const Move& move)
{
	return m_currentBoard.isLegal(move);
}

Move Game::move(int variation) const
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			return m_moveNodes[node].move;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return Move();
}

int Game::moveId(int variation) const
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			return node;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return -1;
}

int Game::currentMoveId() const
{
	return m_currentNode;
}

QString Game::annotation(int variation) const
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			return m_moveNodes[node].annotation;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return QString();
}

NagSet Game::nags(int variation) const
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			return m_moveNodes[node].nags;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return NagSet();
}

QString Game::previousMoveToSan(Game::MoveStringFlags flags)
{
	int originalVariation = currentVariation();

	if (backward()) {
		QString san = moveToSan(flags, originalVariation);
		enterVariation(originalVariation);
		return san;
	} else {
		return QString();
	}
}

QString Game::moveToSan(Game::MoveStringFlags flags, int variation)
{
	QString san;

	//move number
	if (m_currentBoard.toMove() == Black) {
		if (flags & BlackNumbers) {
			san += QString::number(moveNumber()) + "... ";
		}
	} else {
		if (flags & WhiteNumbers) {
			san += QString::number(moveNumber()) + ". ";
		}
	}

	//move
	san += m_currentBoard.moveToSAN(move(variation));

	//nags
	if (flags & Nags) {
		QString nagString = nags(variation).toString();
		if (!nags(variation).count() || nagString.startsWith("!") || nagString.startsWith("?")) {
			san += nagString;
		} else {
			san += " " + nagString;
		}
	}
	return san;
}


bool Game::setAnnotation(QString annotation, int variation)
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			m_moveNodes[node].annotation = annotation;
			return true;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return false;
}

bool Game::addNag(Nag nag, int variation)
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			m_moveNodes[node].nags.addNag(nag);
			return true;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return false;
}

bool Game::setNags(NagSet nags, int variation)
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			m_moveNodes[node].nags = nags;
			return true;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return false;
}

bool Game::atStart() const
{
	return !m_currentNode;
}

bool Game::atEnd() const
{
	return !m_moveNodes[m_currentNode].nextNode;
}

void Game::moveCount(int* moves, int* comments, int* nags)
{
	*moves = *comments = *nags = 0;
	moveCount(m_moveNodes[m_currentNode].nextNode, moves, comments, nags);
}

int Game::ply() const
{
	return m_ply;
}

int Game::plyCount() const
{
	int count = m_ply;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		count++;
		node = m_moveNodes[node].nextNode;
	}

	return count;
}

int Game::currentVariation() const
{
	if (m_currentNode == 0) {
		return 0;
	}

	int count = 0;
	int node = m_moveNodes[m_moveNodes[m_currentNode].previousNode].nextNode;

	while (node != m_currentNode) {
		count++;
		node = m_moveNodes[node].nextVariation;
	}

	return count;
}

int Game::variationCount() const
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		count++;
		node = m_moveNodes[node].nextVariation;
	}

	return count;
}

void Game::moveToStart()
{
	m_currentNode = 0;
	m_ply = 0;
	m_currentBoard = m_startBoard;
	m_history.clear();
}

int Game::moveToPly(int ply)
{
	int diff = ply - m_ply;

	if (diff > 0) {
		forward(diff);
	} else {
		backward(-diff);
	}

	return m_ply;
}

int Game::moveByPly(int diff)
{
	if (diff > 0) {
		return forward(diff);
	} else {
		return backward(-diff);
	}
}

void Game::moveToId(int moveId)
{
	if (moveId < 0)
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
	m_ply = 0;
	m_currentBoard = m_startBoard;
	m_history.clear();

	while (!moveStack.isEmpty()) {
		m_ply++;
		m_history.push(m_currentBoard.doMove(moveStack.pop()));
	}
}

void Game::moveToEnd()
{
	while (m_moveNodes[m_currentNode].nextNode) {
		m_currentNode = m_moveNodes[m_currentNode].nextNode;
		Q_ASSERT(m_currentBoard.isLegal(m_moveNodes[m_currentNode].move));
		m_history.push(m_currentBoard.doMove(m_moveNodes[m_currentNode].move));
		m_ply++;
	}
}

int Game::forward(int count)
{
	int toMove = count;

	while (count && m_moveNodes[m_currentNode].nextNode) {
		m_currentNode = m_moveNodes[m_currentNode].nextNode;
		Q_ASSERT(m_currentBoard.isLegal(m_moveNodes[m_currentNode].move));
		m_history.push(m_currentBoard.doMove(m_moveNodes[m_currentNode].move));
		m_ply++;
		count--;
	}

	return (toMove - count);
}

int Game::backward(int count)
{
	int toMove = count;

	while (count && m_currentNode) {
		m_currentBoard.undoMove(m_moveNodes[m_currentNode].move, m_history.pop());
		m_currentNode = m_moveNodes[m_currentNode].previousNode;
		m_ply--;
		count--;
	}

	return (toMove - count);
}

bool Game::enterVariation(int variation)
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			m_currentNode = node;
			Q_ASSERT(m_currentBoard.isLegal(m_moveNodes[m_currentNode].move));
			m_history.push(m_currentBoard.doMove(m_moveNodes[m_currentNode].move));
			m_ply++;
			return true;
		};
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return false;
}

void Game::exitVariation()
{
	int parentNode = m_moveNodes[m_currentNode].parentNode;
	while (m_currentNode != parentNode) {
		m_currentBoard.undoMove(m_moveNodes[m_currentNode].move, m_history.pop());
		m_currentNode = m_moveNodes[m_currentNode].previousNode;
		m_ply--;
	}
}

int Game::addMove(const Move& move, const QString& annotation, NagSet nags)
{
	//make sure we have space
	if (m_nextFreeNode == m_totalNodeCount) {
		compact();
	}

	//set up node
	m_moveNodes[m_nextFreeNode].move = move;
	m_moveNodes[m_nextFreeNode].annotation = annotation;
	m_moveNodes[m_nextFreeNode].nags = nags;
	m_moveNodes[m_nextFreeNode].nextNode = 0;
	m_moveNodes[m_nextFreeNode].nextVariation = 0;
	m_moveNodes[m_nextFreeNode].parentNode = 0;

	//link back to previous node
	int variation = 0;
	m_moveNodes[m_nextFreeNode].previousNode = m_currentNode;

	//link forward from previous move or last variation
	int lastNode = m_moveNodes[m_currentNode].nextNode;
	if (!lastNode) {
		m_moveNodes[m_currentNode].nextNode = m_nextFreeNode;
		m_moveNodes[m_nextFreeNode].parentNode = m_moveNodes[m_currentNode].parentNode;
	} else {
		variation++;
		while (m_moveNodes[lastNode].nextVariation) {
			lastNode = m_moveNodes[lastNode].nextVariation;
			variation++;
		}
		m_moveNodes[m_nextFreeNode].parentNode = m_currentNode;
		m_moveNodes[lastNode].nextVariation = m_nextFreeNode;
	}

	//node now added
	m_nextFreeNode++;
	return variation;
}

int Game::addMove(const QString& sanMove, const QString& annotation, NagSet nags)
{
	Move move = m_currentBoard.singleMove(sanMove);
	if (!move.isValid()) {
		return -1;
	}
	return addMove(move, annotation, nags);
}

bool Game::replaceMove(const Move& move, const QString& annotation, NagSet nags, int variation)
{
	int count = 0;
	int node;
	node = m_moveNodes[m_currentNode].nextNode;

	if (!node) {
		addMove(move, annotation, nags);
		return true;
	}

	while (node) {
		if (count == variation) {
			//replace node data with new move
			m_moveNodes[node].move = move;
			m_moveNodes[node].annotation = annotation;
			m_moveNodes[node].nags = nags;

			//remove any following nodes by disconnecting them from the tree
			if (m_moveNodes[node].nextNode) {
				m_deletedNodeCount += nodeCount(m_moveNodes[node].nextNode);
				m_moveNodes[node].nextNode = 0;
			}

			return true;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return false;
}

bool Game::replaceMove(const QString& sanMove, const QString& annotation, NagSet nags, int variation)
{
	return replaceMove(m_currentBoard.singleMove(sanMove), annotation, nags, variation);
}

bool Game::promoteVariation(int variation)
{
	if (variation == 0) {
		return false;
	}

	int count = 0;
	int previousNode = 0;
	int mainLine, node;
	node = mainLine = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			m_moveNodes[previousNode].nextVariation = m_moveNodes[node].nextVariation;
			m_moveNodes[node].nextVariation = mainLine;
			m_moveNodes[m_currentNode].nextNode = node;

			//set parent nodes moves in each variation
			int variationNode = mainLine;
			while (variationNode) {
				m_moveNodes[variationNode].parentNode = m_currentNode;
				variationNode = m_moveNodes[variationNode].nextNode;
			}

			variationNode = node;
			while (variationNode) {
				m_moveNodes[node].parentNode = m_moveNodes[m_currentNode].parentNode;
				variationNode = m_moveNodes[variationNode].nextNode;
			}

			return true;
		};
		previousNode = node;
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return false;
}

bool Game::removeVariation(int variation)
{
	int count = 0;
	int previousNode = 0;
	int mainLine, node;
	node = mainLine = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			//effectively remove nodes by disconnecting from tree
			if (previousNode) {
				m_moveNodes[previousNode].nextVariation = m_moveNodes[node].nextVariation;
			} else {
				m_moveNodes[m_currentNode].nextNode = 0;
			}

			//count nodes to update deleted total
			m_deletedNodeCount += nodeCount(node);
			return true;
		};
		previousNode = node;
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return false;
}

void Game::truncateGameEnd()
{
	//effectively remove nodes by disconnecting from tree
	m_deletedNodeCount += nodeCount(m_currentNode) - 1;
	m_moveNodes[m_currentNode].nextNode = 0;
}

bool Game::truncateGameStart(int variation)
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;

	while (node) {
		if (count == variation) {
			//effectively remove nodes by disconnecting from tree
			m_deletedNodeCount += nodeCount(0) - nodeCount(node);
			m_startBoard = m_currentBoard;
			m_moveNodes[0].nextNode = node;
			m_moveNodes[node].previousNode = m_moveNodes[node].parentNode = m_moveNodes[node].nextVariation = 0;
			moveToStart();
			return true;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}

	return false;
}

QString Game::ecoClassify() const
{
	//move to end of main line
	int node = 0;
	Board board = m_startBoard;
	History history;

	while (m_moveNodes[node].nextNode) {
		node = m_moveNodes[node].nextNode;
		history.push(board.doMove(m_moveNodes[node].move));
	}

	//search backwards for the first eco position
	while (node) {
		quint64 key = board.getHashValue();
		if (m_ecoPositions.contains(key)) {
			return m_ecoPositions[key];
		}
		board.undoMove(m_moveNodes[node].move, history.pop());
		node = m_moveNodes[node].previousNode;
	}

	return QString();
}

Board Game::startBoard() const
{
	return m_startBoard;
}


QString Game::startAnnotation() const
{
	return m_startAnnotation;
}

Result Game::result() const
{
	return m_result;
}

void Game::clear()
{
	m_startBoard.fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	m_currentBoard = m_startBoard;
	m_startAnnotation = QString();
	m_result = Unknown;

	m_currentNode = 0;
	m_ply = 0;
	m_startPly = 0;
	m_nextFreeNode = 1;
	m_deletedNodeCount = 0;
	m_moveNodes[0].nextNode = 0;

	m_tags.clear();
	m_history.clear();
}
void Game::clearTags()
{
	m_tags.clear();
	m_history.clear();
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
	m_tags.insert(tag, value);
}

void Game::removeTag(const QString& tag)
{
	m_tags.remove(tag);
}

void Game::setStartBoard(const Board& board, int firstMove)
{
	m_startBoard = board;
	m_currentBoard = board;

	// reset game, otherwise it may be invalid
	m_startAnnotation = QString();
	m_result = Unknown;

	m_currentNode = 0;
	m_ply = 0;
	m_startPly = (firstMove - 1) * 2 + (m_startBoard.toMove() == Black);
	m_nextFreeNode = 1;
	m_deletedNodeCount = 0;
	m_moveNodes[0].nextNode = 0;
}

void Game::setStartBoard(const QString& fen)
{
	m_startBoard.fromFEN(fen);
	m_currentBoard = m_startBoard;
	bool ok;
	int start = fen.section(' ', -1).toInt(&ok);
	if (ok && start > 0)
		m_startPly = (start - 1) * 2 + (m_startBoard.toMove() == Black);
	else m_startPly = 0;
	// reset game, otherwise it may be invalid
	m_startAnnotation = QString();
	m_result = Unknown;

	m_currentNode = 0;
	m_ply = 0;
	m_nextFreeNode = 1;
	m_deletedNodeCount = 0;
	m_moveNodes[0].nextNode = 0;
}

void Game::setStartAnnotation(const QString& annotation)
{
	m_startAnnotation = annotation;
}

void Game::setResult(const Result result)
{
	m_result = result;
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
	//check if empty
	if (m_deletedNodeCount == m_totalNodeCount) {
		m_currentNode = 0;
		m_nextFreeNode = 1;
		return;
	}

	//determine number of nodes to allocate
	int newSize;
	if (m_deletedNodeCount < m_totalNodeCount / 4) {
		newSize = m_totalNodeCount * 2;
	} else {
		newSize = m_totalNodeCount;
	}

	MoveNode* newMoveNodes = new MoveNode[newSize];

	//copy nodes to new storage
	newMoveNodes[0].nextNode = 0;
	newMoveNodes[0].parentNode = 0;
	newMoveNodes[0].nextVariation = 0;
	m_nextFreeNode = 1;
	copyVariation(0, m_moveNodes[0].nextNode, newMoveNodes, m_totalNodeCount);
	//gameDumper();

	m_deletedNodeCount = 0;

	//free old storage and switch to the new
	delete[] m_moveNodes;
	m_moveNodes = newMoveNodes;
	m_totalNodeCount = newSize;
}

void Game::copyVariation(int parentNode, int startNode,	MoveNode* destinationNodes, int endNode)
{
	int previousNode = parentNode;
	int sourceNode = startNode;

	while (sourceNode) {

		//allocate new node
		int destinationNode = m_nextFreeNode++;
		if (destinationNode >= endNode) {
			break;
		}

		//conect node to previous node
		if (!destinationNodes[previousNode].nextNode) {
			destinationNodes[previousNode].nextNode = destinationNode;
		}

		//copy data to new node
		destinationNodes[destinationNode].move = m_moveNodes[sourceNode].move;
		destinationNodes[destinationNode].annotation = m_moveNodes[sourceNode].annotation;
		destinationNodes[destinationNode].nags = m_moveNodes[sourceNode].nags;
		destinationNodes[destinationNode].previousNode = previousNode;
		destinationNodes[destinationNode].nextNode = 0;
		destinationNodes[destinationNode].parentNode = parentNode;
		destinationNodes[destinationNode].nextVariation = 0;

		//check for any variations
		int variationNode = m_moveNodes[sourceNode].nextVariation;
		int previousVariationNode = destinationNode;

		if (variationNode) {
			destinationNodes[previousVariationNode].nextVariation = m_nextFreeNode;
			copyVariation(previousNode, variationNode, destinationNodes, endNode);
			variationNode = m_moveNodes[variationNode].nextVariation;
			previousVariationNode = destinationNodes[previousVariationNode].nextVariation;
		}

		//update m_currentNode when copied
		if (sourceNode == m_currentNode) {
			m_currentNode = destinationNode;
		}

		//advance to next node
		previousNode = destinationNode;
		sourceNode = m_moveNodes[sourceNode].nextNode;
	}
}

void Game::moveCount(int node, int* moves, int* comments, int* nags)
{
	//add this node
	*moves += 1;
	*nags += m_moveNodes[node].nags.count();
	if (m_moveNodes[node].annotation != QString()) {
		*comments += 1;
	}

	//recursively call for next move
	int nextNode = m_moveNodes[node].nextNode;
	if (nextNode) {
		moveCount(nextNode, moves, comments, nags);
	}

	//recursively call for any nextVariation
	int nextVariation = m_moveNodes[node].nextVariation;
	if (nextVariation) {
		moveCount(nextVariation, moves, comments, nags);
	}
}

int Game::nodeCount(int node)
{
	int total = 1; //1 for this node

	//recursively call for next move
	int nextNode = m_moveNodes[node].nextNode;
	if (nextNode) {
		total += nodeCount(nextNode);
	}

	//recursively call for any nextVariation
	int nextVariation = m_moveNodes[node].nextVariation;
	if (nextVariation) {
		total += nodeCount(nextVariation);
	}

	return total;
}

int Game::moveNumber() const
{
	return (m_startPly + m_ply) / 2 + 1;
}

