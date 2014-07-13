/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QtDebug>
#include <QFileInfo>
#include "memorydatabase.h"
#include "settings.h"

MemoryDatabase::MemoryDatabase() : PgnDatabase(false), m_isModified(false)
{
}

MemoryDatabase::~MemoryDatabase()
{
    for(int i = 0; i < m_games.count(); ++i)
    {
        delete m_games[i];
    }
}

bool MemoryDatabase::isReadOnly() const
{
    return false;
}

bool MemoryDatabase::isModified() const
{
    return m_isModified;
}

void MemoryDatabase::setModified(bool b)
{
    m_isModified = b;
}

bool MemoryDatabase::appendGame(const Game& game)
{
    // Add to index
    QMap <QString, QString> tags = game.tags();
    QMap <QString, QString>::const_iterator i = tags.constBegin();
    m_count = m_index.add();
    while(i != tags.constEnd())
    {
        m_index.setTag(i.key(), i.value(), m_count);
        ++i;
    }
    // Upate game array
    Game* newGame = new Game;
    *newGame = game;
    newGame->clearTags();
    m_games.append(newGame);
    ++m_count;
    setModified(true);
    return true;
}

bool MemoryDatabase::remove(GameId gameId)
{
    setModified(true);
    m_index.setDeleted(gameId, true);
    return true;
}

bool MemoryDatabase::undelete(GameId gameId)
{
    setModified(true);
    m_index.setDeleted(gameId, false);
    return true;
}

bool MemoryDatabase::replace(GameId gameId, Game& game)
{
    if(gameId >= m_count)
    {
        return false;
    }
    // Update index
    QMap <QString, QString> tags = game.tags();
    QMap <QString, QString>::const_iterator i = tags.constBegin();
    while(i != tags.constEnd())
    {
        m_index.setTag(i.key(), i.value(), gameId);
        ++i;
    }
    // Upate game array
    *m_games[gameId] = game;
    m_games[gameId]->clearTags();
    setModified(true);
    return true;
}

void MemoryDatabase::loadGameMoves(GameId gameId, Game& game)
{
    if(gameId >= m_count)
    {
        return;
    }
    game = *m_games[gameId];
}

bool MemoryDatabase::loadGame(GameId gameId, Game& game)
{
    if(gameId >= m_count || m_index.deleted(gameId))
    {
        return false;
    }

    game = *m_games[gameId];
    loadGameHeaders(gameId, game);

    return true;
}

void MemoryDatabase::parseGame()
{
    Game* game = new Game;
    QString fen = m_index.tagValue(TagNameFEN, m_count - 1);
    if(fen != "?")
    {
        game->dbSetStartingBoard(fen);
    }
    m_index.setValidFlag(m_count - 1, parseMoves(game));
    m_index.setTag("Length", QString::number((game->plyCount() + 1) / 2), m_count - 1);

    QString eco = game->tag("ECO").left(3);
    if(eco == "?")
    {
        eco.clear();
    }

    if(AppSettings->getValue("/General/automaticECO").toBool())
    {
        if(eco.isEmpty())
        {
            eco = game->ecoClassify().left(3);
            if(!eco.isEmpty())
            {
                game->setTag("ECO", eco);
                m_index.setTag("ECO", eco, m_count - 1);
            }
        }
    }

    m_games.append(game);
}

bool MemoryDatabase::parseFile()
{
    bool ok = parseFileIntern();
    setModified(false); // Undoes changes which stem from ECO parsing, is this expected or unexpected?
    return ok;
}
