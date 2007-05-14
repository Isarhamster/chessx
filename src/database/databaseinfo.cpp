/***************************************************************************
                          datebaseinfo  -  Single database information
                             -------------------
    begin                : 23 Aug 2006
    copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
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
#include "databaseinfo.h"
#include "filter.h"
#include "game.h"
#include "pgndatabase.h"
#include "memorydatabase.h"

DatabaseInfo::DatabaseInfo(const QString& fname)
{
	if (fname.isNull()) {
		m_database = new MemoryDatabase();
		m_database->open(QString());
	} else {
		QFile file(fname);
		if (file.size() < 4000000) {
			m_database = new MemoryDatabase();
		} else {
			m_database = new PgnDatabase();
		}
		m_database->open(fname);
	}
	m_game = new Game;
	m_filter = new Filter(m_database);
	m_index = -1;
	loadGame(0);
}

DatabaseInfo::~DatabaseInfo()
{
	delete m_filter;
	delete m_database;
	delete m_game;
}

bool DatabaseInfo::loadGame(int index)
{
	if (!m_database)
		return false;
	if (m_index == index)
		return true;
	if (!m_database->loadGame(index, *m_game))
		return false;
	m_index = index;
	m_game->moveToPly(m_filter->gamePosition(index) - 1);
	return true;
}

QString DatabaseInfo::name() const
{
	QString name = m_database->filename().section('/', -1);
	int ext = name.lastIndexOf('.');
	if (ext > name.length() - 5)
		name = name.left(ext);
	return name;
}

