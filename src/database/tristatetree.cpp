/***************************************************************************
                          tristatetree.cpp  -  Tri-state tree class
                             -------------------
    begin                : 21 March 2006
    copyright            : (C) 2006 William Hoggarth
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

#include "tristatetree.h"

TriStateTree::TriStateTree()
{
	m_state = Unknown;
	m_nodeCount = 0;
	m_nodes = 0;
	m_leafCount = 0;
	m_leafs = 0;
}

TriStateTree::TriStateTree(const Query& query)
{
	m_state = Unknown;
	m_nodeCount = query.count();
	m_nodes = new Node[m_nodeCount];
	m_leafCount = 0;
	m_leafs = new Node*[m_nodeCount];
	int stackTop = -1;
	Node* nodeStack[m_nodeCount];
	
	for(int element = 0; element < m_nodeCount; element++) {
		if(query.isElementSearch(element)) {
			/* search == leaf node, add to stack & add to list of leaf nodes */
			m_nodes[element].m_state = Unknown;
			m_nodes[element].m_operator = Search::NullOperator;
			m_nodes[element].m_parent = 0;
			m_nodes[element].m_leftChild = m_nodes[element].m_rightChild = 0;
			nodeStack[++stackTop] = &m_nodes[element];
			m_leafs[m_leafCount++] = &m_nodes[element];
		} else {
			/* operator == branch node, pop children off stack & add to stack */
			m_nodes[element].m_state = Unknown;
			m_nodes[element].m_operator = query.searchOperator(element);
			m_nodes[element].m_parent = 0;
			if(m_nodes[element].m_operator == Search::Not) {
				m_nodes[element].m_rightChild = 0;
			} else {
				m_nodes[element].m_rightChild = nodeStack[stackTop--];
				m_nodes[element].m_rightChild->m_parent = &m_nodes[element];
			}
			m_nodes[element].m_leftChild = nodeStack[stackTop--];
			m_nodes[element].m_leftChild->m_parent = &m_nodes[element];
			nodeStack[++stackTop] = &m_nodes[element];
		}
	}
	
	//stack should now be empty
	Q_ASSERT(stackTop == 0);
}

TriStateTree::TriStateTree(const TriStateTree& tree)
{
	//assign data members
	m_state = tree.m_state;
	m_nodeCount = tree.m_nodeCount;
	m_leafCount = tree.m_leafCount;

	//copy nodes
	m_nodes = new Node[m_nodeCount];
	m_leafs = new Node*[m_nodeCount];
	
	for(int node = 0; node < m_nodeCount; node++) {
		m_nodes[node].m_state = tree.m_nodes[node].m_state;
		m_nodes[node].m_operator = tree.m_nodes[node].m_operator;
		if(tree.m_nodes[node].m_parent) {
			m_nodes[node].m_parent = m_nodes + (tree.m_nodes[node].m_parent - tree.m_nodes);
		} else {
			m_nodes[node].m_parent = 0;
		}
		if(tree.m_nodes[node].m_leftChild) {
			m_nodes[node].m_leftChild = m_nodes + (tree.m_nodes[node].m_leftChild - tree.m_nodes);
		} else {
			m_nodes[node].m_leftChild = 0;
		}
		if(tree.m_nodes[node].m_rightChild) {
			m_nodes[node].m_rightChild = m_nodes + (tree.m_nodes[node].m_rightChild - tree.m_nodes);
	  } else {
			m_nodes[node].m_rightChild = 0;
		}
	}
	
	for(int node = 0; node < m_leafCount; node++) {
		m_leafs[node] = m_nodes + (tree.m_leafs[node] - tree.m_nodes);
	}
}

TriStateTree& TriStateTree::operator=(const TriStateTree& tree)
{
	//assign data members
	m_state = tree.m_state;
	m_nodeCount = tree.m_nodeCount;
	m_leafCount = tree.m_leafCount;

	//copy nodes
	Node* nodes = new Node[m_nodeCount];
	Node** leafs = new Node*[m_nodeCount];
	
	for(int node = 0; node < m_nodeCount; node++) {
		nodes[node].m_state = tree.m_nodes[node].m_state;
		nodes[node].m_operator = tree.m_nodes[node].m_operator;
		if(tree.m_nodes[node].m_parent) {
			nodes[node].m_parent = nodes + (tree.m_nodes[node].m_parent - tree.m_nodes);
		} else {
			nodes[node].m_parent = 0;
		}
		if(tree.m_nodes[node].m_leftChild) {
			nodes[node].m_leftChild = nodes + (tree.m_nodes[node].m_leftChild - tree.m_nodes);
		} else {
			nodes[node].m_leftChild = 0;
		}
		if(tree.m_nodes[node].m_rightChild) {
			nodes[node].m_rightChild = nodes + (tree.m_nodes[node].m_rightChild - tree.m_nodes);
		} else {
			nodes[node].m_rightChild = 0;
		}
	}
	
	for(int node = 0; node < m_leafCount; node++) {
		leafs[node] = nodes + (tree.m_leafs[node] - tree.m_nodes);
	}
	
	delete[]m_nodes;
	m_nodes = nodes;
	delete[]m_leafs;
	m_leafs = leafs;
	
	return *this;
}

TriStateTree::~TriStateTree()
{
	delete[] m_nodes;
	delete[] m_leafs;
}

TriStateTree::State TriStateTree::state() const
{
	return m_state;
}
		
TriStateTree::State TriStateTree::state(int leaf) const
{
	if(leaf < m_leafCount) {
		return m_leafs[leaf]->m_state;
	} else {
		return Unknown;
	}
}

int TriStateTree::leafCount() const
{
	return m_leafCount;
}

TriStateTree::State TriStateTree::setState(int leaf, bool state)
{
	if(leaf < m_leafCount) {
		m_leafs[leaf]->m_state = State(1 + state);
		if(m_leafs[leaf]->m_parent) {
			m_state = update(m_leafs[leaf]->m_parent);
		} else {
			m_state = m_leafs[leaf]->m_state;
		}
	} 
	
	return m_state;
}

TriStateTree::State TriStateTree::setState(int leaf, State state)
{
	if(leaf < m_leafCount) {
		m_leafs[leaf]->m_state = state;
		if(m_leafs[leaf]->m_parent) {
			m_state = update(m_leafs[leaf]->m_parent);
		} else {
			m_state = m_leafs[leaf]->m_state;
		}
	} 
	
	return m_state;
}

void TriStateTree::clear()
{
	m_state = Unknown;
	for(int node = 0; node < m_nodeCount; node++) {
		m_nodes[node].m_state = Unknown;
	}
}	

TriStateTree::State TriStateTree::update(Node* node)
{
	State oldState = node->m_state;
	
	State leftState, rightState;
	leftState = node->m_leftChild->m_state;
	if(node->m_rightChild) {
		rightState = node->m_rightChild->m_state;
	} else {
		rightState = Unknown;
	}
	
	switch(node->m_operator) {
		
		case Search::Not:
			switch(leftState) {
				case Unknown:
					node->m_state = Unknown;
					break;
				case False:
					node->m_state = True;
					break;
				case True:
					node->m_state = False;
					break;
			} 
			break;
		
		case Search::And:
			if(leftState == False || rightState == False) {
				node->m_state = False;
			} else if(leftState == Unknown || rightState == Unknown) {
				node->m_state = Unknown;
			} else {
				node->m_state = True;
			}
			break;
		
		case Search::Or:
		case Search::Add:
			if(leftState == True || rightState == True) {
				node->m_state = True;
			} else if(leftState == Unknown || rightState == Unknown) {
				node->m_state = Unknown;
			} else {
				node->m_state = False;
			}
			break;
		
		case Search::Remove:
			if(leftState == False || rightState == True) {
				node->m_state = False;
			} else if(leftState == Unknown || rightState == Unknown) {
				node->m_state = Unknown;
			} else {
				node->m_state = True;
			}
			break;
		
		default:
			break;
	}
	
	//only update parent node, if this node's state has changed
	if(oldState != node->m_state) {
		if(node->m_parent) {
			return update(node->m_parent);
		} else {
			return node->m_state;
		}
	} else {
		return m_state;
	}
}
