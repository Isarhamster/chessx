/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net       *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QFile>
#include <QFileInfo>
#include <QtDebug>
#include "database.h"

Database::Database() : m_break(false)
{
    connect(&m_index, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
}

Database::~Database()
{
}

bool Database::isUtf8() const
{
    return m_utf8;
}

void Database::setUtf8(bool utf8)
{
    m_utf8 = utf8;
}

void Database::lock()
{
    mutex.lock();
}

void Database::unlock()
{
    mutex.unlock();
}



Index* Database::index()
{
    return &m_index;
}

const Index* Database::index() const
{
    return &m_index;
}

int Database::count()
{
    return 0;
}

bool Database::isModified() const
{
    return false;
}

bool Database::getValidFlag(GameId id) const
{
    return m_index.isValidFlag(id);
}

bool Database::deleted(GameId id) const
{
    return m_index.deleted(id);
}

QString Database::name() const
{
    QFileInfo fi(filename());
    return fi.completeBaseName();
}

bool Database::isReadOnly() const
{
    return true;
}

bool Database::loadGame(int index, Game &game)
{
    return false;
}

void Database::loadGameHeaders(GameId id, Game &game)
{
    m_index.loadGameHeaders(id, game);
}

void Database::loadGameHeader(GameId id, Game &game, const QString &tag)
{
    m_index.loadGameHeader(id, game, tag);
}

bool Database::replace(int, Game &)
{
    return false;
}

bool Database::appendGame(const Game &)
{
    return false;
}

bool Database::undelete(int)
{
    return false;
}

bool Database::remove(const Filter &)
{
    return false;
}

bool Database::remove(int)
{
    return false;
}

