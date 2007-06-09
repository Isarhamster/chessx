/***************************************************************************
                          OpeningTree
                             -------------------
    begin                : 9 Jun 2007
    copyright            : (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __OPENINGTREE_H__
#define __OPENINGTREE_H__

#include <QAbstractTableModel>
#include <QMap>
#include "filter.h"
#include "game.h"

class MoveData
{
public:
	MoveData();
	void addGame(Game& g, Color c);
	int percentage() const;
	int averageRating() const;
	QString move;
	unsigned count;
	unsigned result[4];
	long rating;
	unsigned rated;
	friend bool operator<(const MoveData& m1, const MoveData& m2);
};







/** @ingroup Search
The OpeningTree class is a class to calculate opening tree for given position. */

class OpeningTree //: public QAbstractTableModel
{
public:
	/** Empty constructor */
	OpeningTree()	{}
	/** Initialized constructor, performing automatic update */
	OpeningTree(Filter& f, const Board& b)	{update(f, b);}
	/** Calculate opening tree from given position, using given filter. It sets
	the filter to contain only game matching position @p b . */
	void update(Filter& f, const Board& b);
	/** Debug string */
	QString debug();
private:
	QList<MoveData> m_moves;
};

#endif

