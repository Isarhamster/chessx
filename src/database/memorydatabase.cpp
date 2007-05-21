/***************************************************************************
                          filename - description
                             -------------------
    begin                : 21/03/2007
    copyright            : (C) 2007 Marius Roets <saidinwielder@sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtDebug>
#include "memorydatabase.h"

MemoryDatabase::MemoryDatabase()
{}
MemoryDatabase::~MemoryDatabase()
{
	for (int i = 0; i < m_games.count(); ++i) {
		delete m_games[i];
	}
}
bool MemoryDatabase::appendGame(Game& game)
{
	int index = m_count;
	// Add to index
	QMap <QString, QString> tags = game.tags();
	QMap <QString, QString>::const_iterator i = tags.constBegin();
	while (i != tags.constEnd()) {
		m_index.setTag(i.key(), i.value(), index);
		++i;
	}
	// Upate game array
	Game* newGame = new Game;
	*newGame = game;
	newGame->clearTags();
	m_games.append(newGame);
	m_count++;
	return true;
}
bool MemoryDatabase::remove(int)
{
	return false;
}
bool MemoryDatabase::remove(const Filter&)
{
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
	return true;
}
bool MemoryDatabase::loadGame(int index, Game& game)
{
	if (!m_isOpen || index >= m_count) {
		return false;
	}
	game.clear();
	game = *m_games[index];
	loadGameHeaders(index, game);

	return true;
}
bool MemoryDatabase::parseFile()
{
	m_index.setCacheEnabled(true);
	//indexing game positions in the file, game contents are ignored
	while (!m_file->atEnd()) {
		skipJunk();
		addOffset();
		parseTagsIntoIndex(); // This will parse the tags into memory
		Game* game = new Game;
		QString fen = m_index.tagValue(TagFEN, m_count - 1);
		if (fen != "?") {
			Board board;
			board.fromFEN(fen);
			game->setStartBoard(board);
		}
		parseMoves(game);
		m_index.setTag("Length", QString::number((game->plyCount() + 1) / 2),m_count-1);
		m_games.append(game);
	}
	m_index.setCacheEnabled(false);
	return true;
}
