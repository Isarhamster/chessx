/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 20/02/2007
    copyright            : (C) 2007 Marius Roets <saidinwielder@users.sourceforge.net>
                           (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tagvalues.h"

QVariant TagValues::data(const QModelIndex & index, int role) const
{
	if ((role == Qt::EditRole || role == Qt::DisplayRole) && index.isValid() && index.row() < count())
		return QVariant(value(index.row()));
	return QVariant();
}

QModelIndex TagValues::index(int row, int column, const QModelIndex& parent) const
{
	if (parent.isValid())
		return QModelIndex();
	return createIndex(row, column);
}

int TagValues::columnCount(const QModelIndex&) const
{
	return 1;
}

int TagValues::rowCount(const QModelIndex& parent) const
{
	return parent.isValid() ? 0 : count();
}

QModelIndex TagValues::parent(const QModelIndex&) const
{
	return QModelIndex();
}

void TagValues::appendToStream(const QString&, QDataStream &)
{
#warning appendToStream is not implemented yet.
}

