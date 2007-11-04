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
#include "chessxdatabase.h"
#include "memorydatabase.h"

DatabaseInfo::DatabaseInfo(const QString& fname)
{
	if (fname.isNull()) {
		m_database = new MemoryDatabase();
		m_database->open(QString());
	} else {
		QFile file(fname);
		if (fname.endsWith(".cxd"))
         m_database = new ChessXDatabase();
		else if (file.size() < 10000000) m_database = new MemoryDatabase();
		else m_database = new PgnDatabase();
		m_database->open(fname);
	}
	m_filter = new Filter(m_database);
	m_index = NewGame;
	if (!loadGame(0))
		newGame();
}

DatabaseInfo::~DatabaseInfo()
{
	delete m_filter;
	delete m_database;
}

bool DatabaseInfo::loadGame(int index)
{
	if (m_index == index)
		return true;
	if (!m_database || index < 0 || index >= m_database->count())
		return false;
	if (!m_database->loadGame(index, m_game))
		return false;
	m_index = index;
	m_game.moveToPly(m_filter->gamePosition(index) - 1);
	m_game.setModified(false);
	return true;
}

void DatabaseInfo::newGame()
{
	m_game.clear();
	m_index = NewGame;
}

QString DatabaseInfo::name() const
{
	QString name = m_database->filename().section('/', -1);
	int ext = name.lastIndexOf('.');
	if (ext > name.length() - 5)
		name = name.left(ext);
	return name;
}

bool DatabaseInfo::saveGame()
{
	if (m_database->isReadOnly())
		return false;
	if (m_index < m_database->count() && m_index >= 0)
		return m_database->replace(m_index, m_game);
	else if (m_index == NewGame) {
		bool ok = m_database->appendGame(m_game);
		if (ok)
			m_filter->resize(m_database->count(), 1);
		return ok;
	}
	else return false;
}

