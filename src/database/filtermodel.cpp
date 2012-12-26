/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "database.h"
#include "filtermodel.h"

#include <QtGui>

FilterModel::FilterModel(Filter* filter, QObject* parent)
        : QAbstractItemModel(parent), m_filter(filter), m_gameIndex(-1)
{
	m_columnNames << tr("Nr")
	<< tr("White")
	<< tr("WhiteElo")
	<< tr("Black")
	<< tr("BlackElo")
	<< tr("Event")
	<< tr("Site")
	<< tr("Round")
	<< tr("Date")
	<< tr("Result")
	<< tr("ECO")
    << tr("Moves");

	m_columnTags << "Nr"
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
    << "Length";

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
    if (index.isValid() && index.row() < m_filter->count())
    {
		int i = m_filter->indexToGame(index.row());
		if (i != -1) {
			if (i != m_gameIndex) {
// rico: it would perhaps be better to read here only header information that is
// currently used and not the whole header information.
				m_filter->database()->loadGameHeaders(i, *m_game);
				m_gameIndex = i;
			}
            if (role == Qt::DisplayRole)
            {
                if (index.column() == 0)
                    return i + 1;
                else {
                    QString tag = m_game->tag(m_columnTags.at(index.column()));
                    if (tag == "?") tag.clear();
                    return tag;
                }
            }
            else if (role == Qt::ForegroundRole)
            {
                if (!m_filter->database()->getValidFlag(i))
                {
                    QVariant v = QColor(Qt::red);
                    return v;
                }
                QVariant v = QColor(Qt::black);
                return v;
            }
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
    return createIndex(row, column, (void*) 0);
}

void FilterModel::setFilter(Filter* filter)
{
    beginResetModel();
	m_filter = filter;
	m_gameIndex = -1;
    endResetModel();
}

Filter* FilterModel::filter()
{
	return m_filter;
}



