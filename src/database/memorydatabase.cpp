/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QtCore>
#include <QFileInfo>
#include <QMutexLocker>
#include "memorydatabase.h"
#include "settings.h"
#include "tags.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

MemoryDatabase::MemoryDatabase() : PgnDatabase(false), m_isModified(false)
{
}

MemoryDatabase::~MemoryDatabase()
{
    clear();
}

void MemoryDatabase::clear()
{
    for(int i = 0; i < m_games.count(); ++i)
    {
        delete m_games[i];
    }
    m_games.clear();
    m_index.clear();
    m_isModified = false;
    m_count = 0;
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
    emit dirtyChanged(m_isModified);
}

bool MemoryDatabase::appendGame(const Game& game)
{
    QWriteLocker m(&m_mutex);
    // Add to index
    m_count = m_index.add();
    setTagsToIndex(game, m_count);

    // Upate game array
    Game* newGame = new Game;
    *newGame = game;
    newGame->clearTags();
    newGame->unmountBoard();
    m_games.append(newGame);
    ++m_count;
    setModified(true);
    return true;
}

bool MemoryDatabase::remove(GameId gameId)
{
    m_index.setDeleted(gameId, true);
    setModified(true);
    return true;
}

bool MemoryDatabase::undelete(GameId gameId)
{
    m_index.setDeleted(gameId, false);
    setModified(true);
    return true;
}

bool MemoryDatabase::replace(GameId gameId, Game& game)
{
    QWriteLocker m(&m_mutex);
    if(gameId >= m_count)
    {
        return false;
    }
    // Update index
    setTagsToIndex(game, gameId);

    // Upate game array
    *m_games[gameId] = game;
    m_games[gameId]->clearTags();
    m_games[gameId]->unmountBoard();
    setModified(true);
    return true;
}

void MemoryDatabase::loadGameMoves(GameId gameId, Game& game)
{
    QReadLocker m(&m_mutex);
    if(gameId >= m_count)
    {
        return;
    }
    game = *m_games[gameId];
}

int MemoryDatabase::findPosition(GameId index, const Board &position)
{
    Game g;
    loadGameMoves(index, g);
    return g.findPosition(position);
}


bool MemoryDatabase::loadGame(GameId gameId, Game& game)
{
    QReadLocker m(&m_mutex);
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
    QWriteLocker m(&m_mutex);
    Game* game = new Game;

    QString fen = m_index.tagValue(TagNameFEN, m_count - 1);
    QString variant = m_index.tagValue(TagNameVariant, m_count - 1).toLower();
    bool chess960 = (variant.startsWith("fischer") || variant.endsWith("960"));
    if(fen != "?")
    {
        game->dbSetStartingBoard(fen, chess960);
    }
    m_index.setValidFlag(m_count - 1, parseMoves(game));

    QString valLength = QString::number((game->plyCount() + 1) / 2);
    m_index.setTag(TagNameLength, valLength, m_count - 1);
    game->setTag(TagNameLength, valLength);

    QString eco = game->tag(TagNameECO).left(3);
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
                game->setTag(TagNameECO, eco);
                m_index.setTag(TagNameECO, eco, m_count - 1);
            }
        }
    }

    game->unmountBoard();
    m_games.append(game);
}

bool MemoryDatabase::parseFile()
{
    bool ok = parseFileIntern();
    return ok;
}
