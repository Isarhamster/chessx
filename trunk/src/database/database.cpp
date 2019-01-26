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

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

Database::Database() : m_break(false), m_utf8(false)
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

Index* Database::index()
{
    return &m_index;
}

const Index* Database::index() const
{
    return &m_index;
}

quint64 Database::count() const
{
    return 0;
}

bool Database::isModified() const
{
    return false;
}

bool Database::getValidFlag(GameId gameId) const
{
    return m_index.isValidFlag(gameId);
}

bool Database::deleted(GameId gameId) const
{
    return m_index.deleted(gameId);
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

bool Database::loadGame(GameId /*gameId*/, Game& /*game*/)
{
    return false;
}

void Database::loadGameHeaders(GameId gameId, Game &game) const
{
    m_index.loadGameHeaders(gameId, game);
}

void Database::loadGameHeader(GameId gameId, Game &game, const QString &tag) const
{
    m_index.loadGameHeader(gameId, game, tag);
}

QString Database::tagValue(GameId gameId, const QString &tag) const
{
    return m_index.tagValue(tag, gameId);
}

bool Database::replace(GameId, Game &)
{
    return false;
}

bool Database::appendGame(const Game &)
{
    return false;
}

bool Database::undelete(GameId)
{
    return false;
}

bool Database::remove(const Filter &)
{
    return false;
}

bool Database::remove(GameId)
{
    return false;
}

void Database::clear()
{
}

void Database::setTagsToIndex(const Game& game, GameId id)
{
    const TagMap& tags = game.tags();
    TagMap::const_iterator i = tags.constBegin();
    while(i != tags.constEnd())
    {
        m_index.setTag(i.key(), i.value(), id);
        ++i;
    }
}
