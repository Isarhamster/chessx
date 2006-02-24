/***************************************************************************
                          game.cpp - chess game
                             -------------------
    begin                : 27 August 2005
    copyright            : (C) 2005 William Hoggarth
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

#include <string.h> 
 
#include "game.h"

Game::Game()
{
	m_startBoard.fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	m_currentBoard = m_startBoard;
	m_startAnnotation = QString::null;
	m_result = Unknown;

	m_currentNode = 0;
	m_ply = 0;
	m_nextFreeNode = 1;
	m_deletedNodeCount = 0;
	m_totalNodeCount = defaultSize;
	m_moveNodes = new MoveNode[defaultSize];
	m_moveNodes[0].nextNode = 0;
}

Game::~Game()
{
	delete[] m_moveNodes;
}

Board Game::board() const
{
	return m_currentBoard;
}

Move Game::move(int variation) const
{	
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;
	
	while(node) {
		if(count == variation) {
			return m_moveNodes[node].move;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}
	
	return Move();
}

QString Game::annotation(int variation) const
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;
	
	while(node) {
		if(count == variation) {
			return m_moveNodes[node].annotation;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}
	
	return QString::null;
}

int Game::nag(int variation) const
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;
	
	while(node) {
		if(count == variation) {
			return m_moveNodes[node].nag;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}
	
	return -1;
}

bool Game::setAnnotation(QString annotation, int variation)
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;
	
	while(node) {
		if(count == variation) {
			m_moveNodes[node].annotation = annotation;
			return true;
		}
		node = m_moveNodes[node].nextVariation;
		count++;
	}
	
	return false;
}

bool Game::setNag(int nag, int variation)
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;
	
	while(node) {
		if(count == variation) {
			m_moveNodes[node].nag = nag;
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

int Game::ply() const
{
	return m_ply;
}

int Game::plyCount() const
{
	int count = m_ply;
	int node = m_moveNodes[m_currentNode].nextNode;
	
	while(node) {
		count++;
		node = m_moveNodes[node].nextNode;
	}
	
	return count;
}

int Game::variation() const
{
	int count = 0;
	int node = m_moveNodes[m_moveNodes[m_currentNode].previousNode].nextNode;
	
	while(node != m_currentNode) {
		count++;
		node = m_moveNodes[node].nextVariation;
	}
	
	return count;
}

int Game::variationCount() const
{
	int count = 0;
	int node = m_moveNodes[m_currentNode].nextNode;
	
	while(node) {
		count++;
		node = m_moveNodes[node].nextVariation;
	}
	
	return count;
}

void Game::toStart()
{
	m_currentNode = 0;
	m_ply = 0;
	m_currentBoard = m_startBoard;
	m_history.clear();
}

void Game::toEnd()
{
	while(m_moveNodes[m_currentNode].nextNode) {
		m_currentNode = m_moveNodes[m_currentNode].nextNode;
		Q_ASSERT(m_currentBoard.isLegal(m_moveNodes[m_currentNode].move));
		m_history.push(m_currentBoard.doMove(m_moveNodes[m_currentNode].move));
		m_ply++;
	}
}

int Game::forward(int count)
{
	int toMove = count;
	
	while(count && m_moveNodes[m_currentNode].nextNode) {
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
	
	while(count && m_currentNode) {
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
	
	while(node) {
		if(count == variation) {
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
	while(m_currentNode != parentNode) {
		m_currentBoard.undoMove(m_moveNodes[m_currentNode].move, m_history.pop());
		m_currentNode = m_moveNodes[m_currentNode].previousNode;
		m_ply--;
	} 
}

int Game::addMove(const Move& move, const QString& annotation, int nag)
{
	//make sure we have space
	if(m_nextFreeNode == m_totalNodeCount) {
		compact();
	}
	
	//set up node
	m_moveNodes[m_nextFreeNode].move = move;
	m_moveNodes[m_nextFreeNode].annotation = annotation;
	m_moveNodes[m_nextFreeNode].nag = nag;
	m_moveNodes[m_nextFreeNode].nextNode = 0;
	m_moveNodes[m_nextFreeNode].nextVariation = 0;
	
	//link back to previous node
	int variation = 0;
	if(atStart()) {
		m_moveNodes[m_nextFreeNode].previousNode = 0;
		m_moveNodes[m_nextFreeNode].parentNode = 0;
		m_moveNodes[m_currentNode].nextNode = m_nextFreeNode;
	} else {
		m_moveNodes[m_nextFreeNode].previousNode = m_currentNode;
		
		//link forward from previous move or last variation
		int lastNode = m_moveNodes[m_currentNode].nextNode;
		if(!lastNode) {
			m_moveNodes[m_currentNode].nextNode = m_nextFreeNode;
			m_moveNodes[m_nextFreeNode].parentNode = m_moveNodes[m_currentNode].parentNode;
		} else {
			variation++;
			while(m_moveNodes[lastNode].nextVariation) {
				lastNode = m_moveNodes[lastNode].nextVariation;
				variation++;
			}
			m_moveNodes[m_nextFreeNode].parentNode = m_currentNode;
			m_moveNodes[lastNode].nextVariation = m_nextFreeNode;
		}
	}

	//node now added
	m_nextFreeNode++;
	return variation;
}

int Game::addMove(const QString& sanMove, const QString& annotation, int nag)
{
	return addMove(m_currentBoard.singleMove(sanMove), annotation, nag);
}

bool Game::promoteVariation(int variation)
{
	if(variation == 0) {
		return false;
	}
	
	int count = 0;
	int previousNode = 0;
	int mainLine, node;
	node = mainLine = m_moveNodes[m_currentNode].nextNode;
	
	while(node) {
		if(count == variation) {
			m_moveNodes[previousNode].nextVariation = m_moveNodes[node].nextVariation;
			m_moveNodes[node].nextVariation = mainLine;
			m_moveNodes[m_currentNode].nextNode = node;
			
			//set parent nodes moves in each variation
			int variationNode = mainLine;
			while(variationNode) {
				m_moveNodes[variationNode].parentNode = m_currentNode;
				variationNode = m_moveNodes[variationNode].nextNode;
			}
			
			variationNode = node;
			while(variationNode) {
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

bool Game::removeMove(int variation)
{
	int count = 0;
	int previousNode = 0;
	int mainLine, node;
	node = mainLine = m_moveNodes[m_currentNode].nextNode;
	
	while(node) {
		if(count == variation) {
			//effectively remove nodes by disconnecting from tree
			if(previousNode) {
				m_moveNodes[previousNode].nextVariation = m_moveNodes[node].nextVariation;
			} else {
				m_moveNodes[m_currentNode].nextNode = 0;
			}
			
			//count nodes to update deleted total
			m_deletedNodeCount += countNodes(node);
			return true;
		};
		previousNode = node;
		node = m_moveNodes[node].nextVariation;
		count++;
	}
	
	return false;
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

		QString Game::tag(const QString& tag) const
{
	return m_tags[tag];
}

void Game::setTag(const QString& tag, const QString& value)
{
	m_tags.insert(tag, value);
}

void Game::removeTag(const QString& tag)
{
	m_tags.remove(tag);
}

void Game::setStartBoard(const Board& board)
{
	m_startBoard = board;
}

void Game::setStartAnnotation(const QString& annotation)
{
	m_startAnnotation = annotation;
}

void Game::setResult(const Result result)
{
	m_result = result;
}

void Game::compact()
{
	//check if empty
	if(m_deletedNodeCount == m_totalNodeCount) {
		m_currentNode = 0;
		m_nextFreeNode = 1;
		return;
	}
	
	//determine number of nodes to allocate
	int newSize;
	if(m_deletedNodeCount < m_totalNodeCount / 4) {
		newSize = m_totalNodeCount * 2;
	} else {
		newSize = m_totalNodeCount;
	}
	
	MoveNode* newMoveNodes = new MoveNode[newSize];
	
	//copy nodes to new storage
	newMoveNodes[0].nextNode = 0;
	m_nextFreeNode = 1;
	copyVariation(0, m_moveNodes[0].nextNode, newMoveNodes);

	m_deletedNodeCount = 0;
	
	//free old storage and switch to the new
	delete[] m_moveNodes;
	m_moveNodes = newMoveNodes;
	m_totalNodeCount = newSize;
}

void Game::copyVariation(int parentNode, int startNode,	MoveNode* destinationNodes)
{
	int previousNode = parentNode;
	int sourceNode = startNode;
	
	while(sourceNode) {
		
		//allocate new node
		int destinationNode = m_nextFreeNode++;
		
		//conect node to previous node
		if(!destinationNodes[previousNode].nextNode) {
		destinationNodes[previousNode].nextNode = destinationNode;
		}
		
		//copy data to new node
		destinationNodes[destinationNode].move = m_moveNodes[sourceNode].move;
		destinationNodes[destinationNode].annotation = m_moveNodes[sourceNode].annotation;
		destinationNodes[destinationNode].nag = m_moveNodes[sourceNode].nag;
		destinationNodes[destinationNode].previousNode = previousNode;
		destinationNodes[destinationNode].nextNode = 0;
		destinationNodes[destinationNode].parentNode = parentNode;
		destinationNodes[destinationNode].nextVariation = 0;
		
		//check for any variations
		int variationNode = m_moveNodes[sourceNode].nextVariation;
		int previousVariationNode = destinationNode;
		
		while(variationNode) {
			destinationNodes[previousVariationNode].nextVariation = m_nextFreeNode;
			copyVariation(previousNode, variationNode, destinationNodes);
			variationNode = m_moveNodes[variationNode].nextVariation;
			previousVariationNode = destinationNodes[previousVariationNode].nextVariation;
		}
		
		//update m_currentNode when copied
		if(sourceNode == m_currentNode) {
			m_currentNode = destinationNode;
		}
		
		//advance to next node
		previousNode = destinationNode;
		sourceNode = m_moveNodes[sourceNode].nextNode;
	}
}

int Game::countNodes(int node)
{
	int total = 1; //1 for this node
	
	//recursively call for next move
	int nextNode = m_moveNodes[node].nextNode;
	if(nextNode) {
		total += countNodes(nextNode);
	}
	
	//recursively call for any nextVariation
	int nextVariation = m_moveNodes[node].nextVariation;
	if(nextVariation) {
		total += countNodes(nextVariation);
	}
	
	return total;
}

