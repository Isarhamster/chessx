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

MemoryDatabase::MemoryDatabase() : m_isModified(false)
{
}

MemoryDatabase::~MemoryDatabase()
{
	for (int i = 0; i < m_games.count(); ++i) {
		delete m_games[i];
	}
}

bool MemoryDatabase::appendGame(const Game& game)
{
	// Add to index
	QMap <QString, QString> tags = game.tags();
	QMap <QString, QString>::const_iterator i = tags.constBegin();
	while (i != tags.constEnd()) {
        m_index.setTag(i.key(), i.value(), m_count);
		++i;
	}
	// Upate game array
	Game* newGame = new Game;
	*newGame = game;
	newGame->clearTags();
	m_games.append(newGame);
    ++m_count;
	m_isModified = true;
	return true;
}

bool MemoryDatabase::remove(int)
{
//	m_isModified = true;
	return false;
}

bool MemoryDatabase::remove(const Filter&)
{
//	m_isModified = true;
	return false;
}

bool MemoryDatabase::replace(int index, Game& game)
{
	if (index >= m_count) {
		return false;
	}
	// Update index
	QMap <QString, QString> tags = game.tags();
	QMap <QString, QString>::const_iterator i = tags.constBegin();
	while (i != tags.constEnd()) {
		m_index.setTag(i.key(), i.value(), index);
		++i;
	}
	// Upate game array
	*m_games[index] = game;
	m_games[index]->clearTags();
	m_isModified = true;
	return true;
}

void MemoryDatabase::loadGameMoves(int index, Game& game)
{
	if (index >= m_count)
		return;
	game = *m_games[index];
}

bool MemoryDatabase::loadGame(int index, Game& game)
{
	if (index >= m_count)
		return false;
	game = *m_games[index];
	loadGameHeaders(index, game);
	return true;
}

void MemoryDatabase::parseGame()
{
    Game* game = new Game;
    QString fen = m_index.tagValue(TagFEN, m_count - 1);
    if (fen != "?")
        game->setStartingBoard(fen);
    parseMoves(game);
    m_index.setTag("Length", QString::number((game->plyCount() + 1) / 2), m_count - 1);
    m_games.append(game);
}

bool MemoryDatabase::parseFile()
{
    PgnDatabase::parseFile();
	m_isModified = false;
	return true;
}

bool MemoryDatabase::clear()
{
	for (int i = 0; i < m_games.count(); ++i) {
		delete m_games[i];
	}
	m_games.clear();
	m_isModified = true;
	return true;
}
