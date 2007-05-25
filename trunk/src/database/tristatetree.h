/***************************************************************************
                          tristatetree.h  -  Tri-state tree class
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

#ifndef __TRISTATETREE_H__
#define __TRISTATETREE_H__

#include "search.h"
#include "query.h"

/** @ingroup Search
   The TriStateTree class helps facilitate the efficient evaluation of queries.
   The tree is built from a query with each leaf node representing a search,
	 all other nodes representing the search operators (and, or etc). Each
   node can be one of three states: Unknown, False, or True. As the result
   of a search for the a game is discovered, the state for the corresponding
   leaf node is set to true or false. After each update the tree is
   re-evaluated, returning a value of Uknown, False, or True. In some cases
   the value of the tree can be determined before all search results are
	 known. Eg. if the query is A or B, then if A is true then we know the
   result is true without having to evaluate B.
*/

class TriStateTree
{
public:
	typedef enum { Unknown, False, True } State;

	/** Default constructor, creates an empty tree */
	TriStateTree();
	/** Constructs a tree from the given query */
	TriStateTree(const Query& query);
	/** Copy constructor */
	TriStateTree(const TriStateTree& tree);
	/** Assignment operator */
	TriStateTree& operator=(const TriStateTree& tree);
	/** Destructor */
	~TriStateTree();

	/** Returns the state of the tree */
	State state() const;
	/** Returns the state of a leaf node */
	State state(int leaf) const;
	/** Returns the number of leaf nodes */
	int leafCount() const;

	/** Sets the state of a leaf node to true or false, the updated state of the tree is returned */
	State setState(int leaf, bool state);
	/** Sets the state of a leaf node to unknown, false, or true. The updated state of the tree is returned */
	State setState(int leaf, State state);
	/** Resets the status of all nodes to unknown */
	void clear();

private:
	typedef struct node
	{
		State m_state;
		Search::Operator m_operator;
		node* m_parent;
		node* m_leftChild;
		node* m_rightChild;
	}
	Node;

	State update(Node* node);

	State m_state;
	int m_nodeCount;
	Node* m_nodes;
	int m_leafCount;
	Node** m_leafs;
};

#endif
