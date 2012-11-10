/***************************************************************************
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QFile>
#include "databaseinfo.h"
#include "filter.h"
#include "game.h"
#include "pgndatabase.h"
#include "memorydatabase.h"
#include "settings.h"

DatabaseInfo::DatabaseInfo()
{
	m_database = new MemoryDatabase;
    m_filter = new Filter(m_database);
    m_bLoaded = true;
    m_utf8 = false;
    newGame();
}

DatabaseInfo::DatabaseInfo(const QString& fname): m_filter(0), m_index(NewGame)
{
	m_filename = fname;
    m_bLoaded = false;
    m_utf8 = false;
	QFile file(fname);
	if (file.size() < 1024 * 1024 * AppSettings->value("/General/EditLimit", 10).toInt()) 
        m_database = new MemoryDatabase;
	else m_database = new PgnDatabase;
}

void DatabaseInfo::doLoadFile(QString filename)
{
    if (!m_database->open(filename,m_utf8)) {
        emit LoadFinished(this);
        return;
    }
    m_database->parseFile();
    delete m_filter;
    m_filter = new Filter(m_database);
    m_bLoaded = true;
    if (!loadGame(0))
        newGame();
    emit LoadFinished(this);
}

void DatabaseInfo::run()
{
    QFileInfo fi = QFileInfo(m_filename);
    QString fname = fi.canonicalFilePath();
    doLoadFile(fname);
}

bool DatabaseInfo::open(bool utf8)
{
    m_bLoaded = false;
    m_utf8 = utf8;
    start();
    return true;
}

void DatabaseInfo::close()
{
    m_bLoaded = false;
    m_database->m_break = true;
    if (isRunning())
    {
        bool bSuccess = wait(5000);
        if (!bSuccess)
        {
            terminate();
        }
    }
	if (m_database) delete m_database;
	if (m_filter) delete m_filter;
    m_database = NULL;
	m_filter = NULL;
}

DatabaseInfo::~DatabaseInfo()
{
}

bool DatabaseInfo::loadGame(int index, bool reload)
{
    if (!m_bLoaded)
        return false;
	if (!isValid())
		return false;
	if (!reload && m_index == index)
		return true;
	if (!m_database || index < 0 || index >= m_database->count())
		return false;
	if (!m_database->loadGame(index, m_game))
		return false;
	m_index = index;
	m_game.moveToId(m_filter->gamePosition(index)-1);
	m_game.setModified(false);
	return true;
}

void DatabaseInfo::newGame()
{
	m_game.clear();
    m_game.setModified(false);
	m_index = NewGame;
}

bool DatabaseInfo::saveGame()
{
    if (!m_bLoaded)
        return false;
	if (!isValid() || m_database->isReadOnly())
		return false;
	if (m_index < m_database->count() && m_index >= 0)
    {
        if (m_database->replace(m_index, m_game))
        {
            m_game.setModified(false);
            return true;
        }
    }
    else if (m_index == NewGame && m_database->appendGame(m_game))
    {
		m_filter->resize(m_database->count(), 1);
		m_index = m_database->count() - 1;
        m_game.setModified(false);
		return true;
    }
    return false;
}

void DatabaseInfo::resetFilter()
{
	if (m_filter) {
		m_filter->resize(m_database->count());
		m_filter->setAll(1);
	}
}

