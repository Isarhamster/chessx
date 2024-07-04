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
#include "tags.h"

using namespace chessx;

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

IndexX* Database::index()
{
    return &m_index;
}

const IndexX* Database::index() const
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

bool Database::loadGame(GameId /*gameId*/, GameX& /*game*/)
{
    return false;
}

void Database::loadGameHeaders(GameId gameId, GameX &game) const
{
    m_index.loadGameHeaders(gameId, game);
}

void Database::loadGameHeader(GameId gameId, GameX &game, const QString &tag) const
{
    m_index.loadGameHeader(gameId, game, tag);
}

QString Database::tagValue(GameId gameId, const QString &tag) const
{
    return m_index.tagValue(tag, gameId);
}

QString Database::tagValue(GameId gameId, TagIndex tag) const
{
    return m_index.tagValue_byIndex(tag, gameId);
}

void Database::findPosition(const BoardX& position, PositionSearchOptions options, const QList<GameId>& games, QList<MoveId>& output, QMap<Move, MoveData>& stats)
{
    for (auto gameId: games)
    {
        // search for position
        GameX g;
        loadGameMoves(gameId, g);
        const auto& cursor = g.cursor();
        auto moveId = cursor.findPosition(position);
        if ((options & PositionSearch_GameEnd) && !cursor.atGameEnd(moveId))
        {
            moveId = NO_MOVE;
        }

        // report result
        output.append(moveId);

        // update stats
        if (moveId != NO_MOVE)
        {
            // determine played move
            Move move;
            if (!cursor.atGameEnd(moveId))
            {
                move = cursor.move(cursor.nextMove(moveId));
            }

            // update metrics
            auto& md = stats[move];
            if (!md.results)
            {
                if (move.isLegal())
                {
                    md.san = position.moveToSan(move);
                    md.localsan = position.moveToSan(move, true);
                }
                else
                {
                    // game is finished
                    md.localsan = md.san = qApp->translate("MoveData", "[end]");
                }
                md.move = move;
            }

            auto result = m_index.tagValue(TagNameResult, gameId);
            if(result == "1-0")
            {
                md.results.update(WhiteWin);
            }
            else if(result == "1/2-1/2")
            {
                md.results.update(Draw);
            }
            else if(result == "0-1")
            {
                md.results.update(BlackWin);
            }
            else
            {
                md.results.update(ResultUnknown);
            }
            auto elo = m_index.tagValue((position.toMove() == White)? TagNameWhiteElo: TagNameBlackElo, gameId);
            md.rating.update(elo.toInt());
            auto date = m_index.tagValue(TagNameDate, gameId);
            md.year.update(date.section(".", 0, 0).toInt());
        }
    }
}

bool Database::replace(GameId, GameX &)
{
    return false;
}

bool Database::appendGame(const GameX &)
{
    return false;
}

bool Database::undelete(GameId)
{
    return false;
}

bool Database::remove(const FilterX &)
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

void Database::setTagsToIndex(const GameX& game, GameId id)
{
    const TagMap& tags = game.tags();
    TagMap::const_iterator i = tags.constBegin();
    while(i != tags.constEnd())
    {
        m_index.setTag(i.key(), i.value(), id);
        ++i;
    }
}

bool Database::hadBOM() const
{
    return m_hadBOM;
}

void Database::setHadBOM(bool newHadBOM)
{
    m_hadBOM = newHadBOM;
}

void Database::setMissingTagsToIndex(const GameX& game, GameId id)
{
    const TagMap& tags = game.tags();
    TagMap::const_iterator i = tags.constBegin();
    while(i != tags.constEnd())
    {
        if (!m_index.hasTag(i.key(), id))
        {
            m_index.setTag(i.key(), i.value(), id);
        }
        ++i;
    }
}
