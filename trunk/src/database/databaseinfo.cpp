/***************************************************************************
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QFile>
#include <QUndoStack>

#include "databaseinfo.h"
#include "filter.h"
#include "game.h"
#include "memorydatabase.h"
#include "pgndatabase.h"
#include "polyglotdatabase.h"
#include "settings.h"

DatabaseInfo::DatabaseInfo(QUndoGroup* undoGroup)
{
    m_database = new MemoryDatabase;
    m_filter = new Filter(m_database);
    connect(m_filter, SIGNAL(searchProgress(int)), SIGNAL(searchProgress(int)));
    connect(m_filter, SIGNAL(searchFinished()), SIGNAL(searchFinished()));
    m_bLoaded = true;
    m_utf8 = false;
    m_undoStack = new QUndoStack((QObject*)undoGroup);
    newGame();
    connect(m_undoStack, SIGNAL(cleanChanged(bool)), SLOT(dbCleanChanged(bool)));
    connect(&m_game, SIGNAL(signalGameModified(bool,Game,QString)),SLOT(setModified(bool,Game,QString)));
    connect(&m_game, SIGNAL(signalMoveChanged()), SIGNAL(signalMoveChanged()));
}

DatabaseInfo::DatabaseInfo(QUndoGroup* undoGroup, const QString& fname): m_filter(0), m_index(NewGame)
{
    m_filename = fname;
    m_bLoaded = false;
    m_utf8 = false;
    m_undoStack = new QUndoStack((QObject*)undoGroup);
    connect(m_undoStack, SIGNAL(cleanChanged(bool)), SLOT(dbCleanChanged(bool)));
    connect(&m_game, SIGNAL(signalGameModified(bool,Game,QString)),SLOT(setModified(bool,Game,QString)));
    connect(&m_game, SIGNAL(signalMoveChanged()), SIGNAL(signalMoveChanged()));
    QFile file(fname);
    if (IsPolyglotBook())
    {
        m_database = new PolyglotDatabase;
    }
    else if(file.size() < 1024 * 1024 * AppSettings->getValue("/General/EditLimit").toInt())
    {
        m_database = new MemoryDatabase;
    }
    else if(file.size() < INT_MAX)
    {
        m_database = new PgnDatabase(false);
    }
    else
    {
        m_database = new PgnDatabase(true);
    }
}

void DatabaseInfo::doLoadFile(QString filename)
{
    if (m_filter) m_filter->cancel();
    if(!m_database->open(filename, m_utf8))
    {
        emit LoadFinished(this);
        return;
    }
    m_database->parseFile();
    delete m_filter;
    m_filter = new Filter(m_database);
    connect(m_filter, SIGNAL(searchProgress(int)), SIGNAL(searchProgress(int)));
    connect(m_filter, SIGNAL(searchFinished()), SIGNAL(searchFinished()));
    m_bLoaded = true;
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
    if (m_filter)
    {
        m_filter->cancel();
    }
    m_bLoaded = false;
    m_database->m_break = true;
    if(isRunning())
    {
        bool bSuccess = wait(5000);
        if(!bSuccess)
        {
            terminate();
        }
    }

    delete m_database;
    delete m_filter;

    m_database = NULL;
    m_filter = NULL;
    m_undoStack->clear();
}

DatabaseInfo::~DatabaseInfo()
{
}

bool DatabaseInfo::loadGame(int index)
{
    if(!m_bLoaded)
    {
        return false;
    }
    if(!isValid())
    {
        return false;
    }
    if(!m_database || index < 0 || index >= (int)m_database->count())
    {
        return false;
    }
    if(!m_database->loadGame(index, m_game))
    {
        return false;
    }
    m_index = index;
    int n = m_filter->gamePosition(index) - 1;
    if(n < 0)
    {
        n = 0;
    }
    m_game.moveToId(n);
    m_undoStack->clear();
    setModified(false, Game(), "");
    return true;
}

void DatabaseInfo::dbCleanChanged(bool bClean)
{
    emit signalGameModified(!bClean);
}

bool DatabaseInfo::modified() const
{
    return !m_undoStack->isClean();
}

bool DatabaseInfo::gameNeedsSaving() const
{
    return (isValid() && modified() && !m_database->isReadOnly());
}

void DatabaseInfo::setModified(bool modified, const Game& g, QString action)
{
    if (modified)
    {
        Q_ASSERT(!action.isEmpty());
        m_undoStack->push(new GameUndoCommand(this, g, m_game, action));
    }
    else
    {
        m_undoStack->clear();
    }
    emit signalGameModified(!m_undoStack->isClean());
}

QUndoStack *DatabaseInfo::undoStack() const
{
    return m_undoStack;
}

void DatabaseInfo::restoreState(const Game& game)
{
    emit signalRestoreState(game);
}

void DatabaseInfo::newGame()
{
    m_undoStack->clear();
    m_game.clearTags();
    m_game.clear();
    m_index = NewGame;
    setModified(false, Game(), "");
}

bool DatabaseInfo::saveGame()
{
    if(!m_bLoaded)
    {
        return false;
    }
    if(!isValid() || m_database->isReadOnly())
    {
        return false;
    }

    QString eco;
    if(AppSettings->getValue("/General/automaticECO").toBool())
    {
        eco = m_game.ecoClassify().left(3);
        if(!eco.isEmpty())
        {
            m_game.setTag("ECO", eco);
            if(m_index >= 0)
            {
                database()->index()->setTag("ECO", eco, m_index);
            }
        }
    }

    if(m_index < (int)m_database->count() && m_index >= 0)
    {
        if(m_database->replace(m_index, m_game))
        {
            setModified(false, Game(), "");
            m_undoStack->setClean();
            return true;
        }
    }
    else if(m_index == NewGame && m_database->appendGame(m_game))
    {
        m_filter->resize(m_database->count(), 1);
        m_index = m_database->count() - 1;
        if(!eco.isEmpty())
        {
            database()->index()->setTag("ECO", eco, m_index);
        }
        setModified(false, Game(), "");
        m_undoStack->setClean();
        return true;
    }
    return false;
}

void DatabaseInfo::replaceGame(const Game &game)
{
    currentGame() = game;
}

void DatabaseInfo::resetFilter()
{
    if(m_filter)
    {
        m_filter->cancel();
        m_filter->resize(m_database->count());
        m_filter->setAll(1);
    }
}

bool DatabaseInfo::IsLoaded() const
{
    return m_bLoaded;
}

bool DatabaseInfo::IsUtf8() const
{
    return m_utf8;
}

bool DatabaseInfo::IsPGN() const
{
    if (m_filename.isEmpty())
    {
        return true;
    }
    QFileInfo fi(m_filename);
    return (fi.suffix() == "pgn");
}

bool DatabaseInfo::IsPolyglotBook() const
{
    QFileInfo fi(m_filename);
    return (fi.suffix() == "bin");
}

bool DatabaseInfo::IsBook() const
{
    // Add here if more book formats come in
    return IsPolyglotBook();
}
