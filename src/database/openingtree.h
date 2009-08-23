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
	enum MoveType {StandardMove, GameEnd};
	MoveData();
	void addGame(Game& g, Color c, MoveType movetype = StandardMove);
	double percentage() const;
	int averageRating() const;
	int averageYear() const;
	QString move;
	unsigned count;
	unsigned result[4];
	long rating;
	unsigned rated;
	long year;
	unsigned dated;
	friend bool operator<(const MoveData& m1, const MoveData& m2);
	friend bool compareMove(const MoveData& m1, const MoveData& m2);
	friend bool compareScore(const MoveData& m1, const MoveData& m2);
	friend bool compareRating(const MoveData& m1, const MoveData& m2);
	friend bool compareYear(const MoveData& m1, const MoveData& m2);
};




/** @ingroup Search
The OpeningTree class is a class to calculate opening tree for given position. */

class OpeningTree : public QAbstractTableModel
{
public:
	/** Empty constructor. */
	OpeningTree();
	/** Initialized constructor, performing automatic update */
	OpeningTree(Filter& f, const Board& b);
	/** Calculate opening tree from given position, using given filter. It sets
	the filter to contain only game matching position @p b . */
	void update(Filter& f, const Board& b);
	/** Debug string */
	QString debug();
	/** Returns the number of moves in the Opening Tree */
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	/** Returns the number of columns of the Opening Tree */
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	/** Returns an item of data given the item 'index' */
	virtual QVariant data(const QModelIndex &index, int role) const;
	/** Returns the header information for header 'section' */
	virtual QVariant headerData(int section, Qt::Orientation orientation,
										 int role = Qt::DisplayRole) const;
	/** Support sorting. */
	virtual void sort(int column, Qt::SortOrder order);
	/** Support sorting. */
	virtual void sort();
	/** Move leading to given entry. */
	QString move(const QModelIndex& index) const;
private:
	QList<MoveData> m_moves;
	unsigned m_games;
	QStringList m_names;
	int m_sortcolumn;
	Qt::SortOrder m_order;
	Board m_board;
	Filter* m_filter;
};

#endif

