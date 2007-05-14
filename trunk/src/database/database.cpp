/***************************************************************************
                          database.cpp  -  abstract database class
                             -------------------
    begin                : 9 April 2006
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

#include <QFile>
#include <QtDebug>
#include "database.h"

Database::~Database()
{}

bool Database::loadGameHeaders(int index, Game& game)
{
	QList <QPair< QString, QString> > gameTags;
	gameTags = m_index.allGameTags(index);

	for (int i = 0; i < gameTags.count(); ++i) {
		game.setTag(gameTags[i].first, gameTags[i].second);
	}

	return true;
}

Index* Database::index()
{
	return &m_index;
}
