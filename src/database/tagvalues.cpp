/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>ccccc           *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "tagvalues.h"

QVariant TagValues::data(const QModelIndex & index, int role) const
{
	if ((role == Qt::EditRole || role == Qt::DisplayRole) && index.isValid() && index.row() < count() - 1)
		return QVariant(value(index.row() + 1));
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
	return parent.isValid() || !count() ? 0 : count() - 1;
}

QModelIndex TagValues::parent(const QModelIndex&) const
{
	return QModelIndex();
}

void TagValues::appendToStream(const QString&, QDataStream &)
{
#ifdef __GNUG__
#warning appendToStream is not implemented yet.
#else // VisualC++
#pragma message("appendToStream is not implemented yet.")
#endif
}

