/***************************************************************************
                          FilterModel.cpp - Provides a model for the Filter class
                                            in term of the Qt filter/model framework
                             -------------------
    begin                : 17/01/2007
    copyright            : (C) 2007 Marius Roets  <saidinwielder@user.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "database.h"
#include "filtermodel.h"

FilterModel::FilterModel(Filter* filter, QObject* parent)
	: QAbstractItemModel(parent), m_filter(filter), m_gameIndex(-1)
{
	m_columnNames << "Nr"
	<< "White"
        << "WhiteElo"
	<< "Black"
        << "BlackElo"
	<< "Event"
	<< "Site"
	<< "Round"
	<< "Date"
	<< "Result"
	<< "ECO"
	<< "PlyCount";
	m_game = new Game;
}

FilterModel::~FilterModel()
{
	delete m_game;
}

int FilterModel::rowCount(const QModelIndex& index) const
{
	if (index.isValid())
		return 0;
	return m_filter->count();
}

int FilterModel::columnCount(const QModelIndex&) const
{
	return m_columnNames.count();
}

QVariant FilterModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole && index.isValid() && index.row() < m_filter->count()) {
		int i = m_filter->indexToGame(index.row());
		if (i != -1) {
			if (i != m_gameIndex)
                          {
// rico: it would perhaps be better to read here only header information that is
// currently used and not the whole header information.
                            m_filter->database()->loadGameHeaders(i, *m_game);
			    m_gameIndex = i;
                          }
			if (index.column() == 0)
				return i + 1;
			else
				return m_game->tag(m_columnNames.at(index.column()));
			//qDebug("Loading game %d: %s-%s", i,
			//    qPrintable(m_game->tag(m_columnNames.at(0))), qPrintable(m_game->tag(m_columnNames.at(1))));
		}
	}
	return QVariant();
}

QVariant FilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal)
		return QString("%1").arg(m_columnNames.at(section));
	else
		return QString("%1").arg(section);
}

QModelIndex FilterModel::index(int row, int column, const QModelIndex& parent) const
{
	if (parent.isValid())
		return QModelIndex();
	return createIndex(row, column);
}

void FilterModel::setFilter(Filter* filter)
{
	m_filter = filter;
	m_gameIndex = -1;
	reset();
}

Filter* FilterModel::filter()
{
	return m_filter;
}



