/***************************************************************************
                          filename - description
                             -------------------
    begin                :
    copyright            : (C) 2006 Marius Roets <saidinwielder@sourceforge.net>
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
#include <QFile>
#include <QDataStream>

#include "index.h"

#define INDEX_ITEM_BUFFER_SIZE 500


Index::Index()
{
	setTagIndexPosition(TagEvent, 0, 2);
	setTagIndexPosition(TagSite, 2, 2);
	setTagIndexPosition(TagDate, 4, 2);
	setTagIndexPosition(TagRound, 6, 2);
	setTagIndexPosition(TagWhite, 8, 4);
	setTagIndexPosition(TagBlack, 12, 4);
	setTagIndexPosition(TagResult, 16, 1);
	setTagIndexPosition(TagPlyCount, 17, 2);
	createIndexItems();
	reallocateIndexItems();
}
Index::~Index()
{
	for (int j = 0; j < m_indexItems.count(); ++j) {
		delete(m_indexItems[j]);
	}

}
void Index::setTag(Tag tag, QString value, int gameId)
{
	// Add value to taglist
	int index = m_tagList.add(tag, value);
	if (index == -1) {
		// It is not a predefined tag. It has to be added using a tag name
		return;
	}
	// Create a index item if it does not exist
	if (!m_indexItems.at(gameId)) {
		add();
	}
	// At index value to itemindex
	m_indexItems[gameId]->set(m_tagIndexPosition[tag].first, m_tagIndexPosition[tag].second, index);

}
void Index::setTag(const QString& tagName, QString value, int gameId)
{
	int index = m_tagList.add(tagName, value);
	while (m_indexItems.count() <= gameId) {
		createIndexItems();
	}
	// At index value to itemindex
	if (!m_tagIndexPosition.contains(m_tagList.tagFromString(tagName))) {
		m_tagIndexPosition.insert((int)m_tagList.tagFromString(tagName), qMakePair((quint8)m_tagIndexSize, (quint8)2));
		reallocateIndexItems(false);
	}
	m_indexItems[gameId]->set(
		m_tagIndexPosition[m_tagList.tagFromString(tagName)].first,
		m_tagIndexPosition[m_tagList.tagFromString(tagName)].second,
		index);
}

GameId Index::add()
{
	int gameId = m_indexItems.count();
	m_indexItems.append(new IndexItem);
	m_indexItems[gameId]->allocate(m_tagIndexSize);
	return gameId;
}
void Index::createIndexItems()
{
	for (int i = 0; i < INDEX_ITEM_BUFFER_SIZE ; ++i) {
		add();
	}
}
void Index::setTagIndexPosition(int tag, quint8 offset, quint8 size)
{
	if (!m_tagIndexPosition.contains(tag)) {
		m_tagIndexPosition.insert(tag, qMakePair(offset, size));
	} else {
		m_tagIndexPosition[tag].first = offset;
		m_tagIndexPosition[tag].second = size;
	}
}
void Index::calculateIndexSize()
{
	m_tagIndexSize = 0;

	// Calculates the size needed
	QMap <int, QPair<quint8 , quint8 > >::const_iterator i = m_tagIndexPosition.constBegin();
	while (i != m_tagIndexPosition.constEnd()) {
		m_tagIndexSize += i.value().second;
		++i;
	}
}
void Index::reallocateIndexItems(bool clear)
{
	calculateIndexSize();

	for (int j = 0; j < m_indexItems.count(); ++j) {
		m_indexItems[j]->allocate(m_tagIndexSize, clear);
	}
}
QString Index::tagValue(Tag tag, int gameId)
{
	return m_tagList[tag]->value(m_indexItems[gameId]->index(
					     m_tagIndexPosition[tag].first,
					     m_tagIndexPosition[tag].second));
}
/*
void Index::write()
{
	QFile file(m_filename);
	if (!file.open(QIODevice::ReadOnly))
		return;
	QDataStream out(&file);
	// I need to write stuff like versioning information, and magic number here

	// Write all the tag values
	m_tagList.write(out);

	// Write the individual index items
	out << m_indexItems.count();
	for (int i = 0 ; i < m_indexItems.count() ; ++i) {
		m_indexItems[i]->write(out);
	}

}
void Index::read()
{
	QFile file(m_filename);
	if (!file.open(QIODevice::ReadOnly))
		return;
	QDataStream in(&file);

	// Read all the tag values
	m_tagList.read(in);

	// Read the individual index items
	clear();
	int indexItemCount;
	in >> indexItemCount;
	for (int i = 0 ; i < indexItemCount ; ++i) {
		add();
		m_indexItems[i]->read(in);
	}

}
void Index::setFilename(const QString& filename)
{
	m_filename = filename;
}
*/
void Index::clear()
{
	for (int i = 0 ; i < m_indexItems.count() ; ++i) {
		delete m_indexItems[i];
	}
	m_indexItems.clear();
}
void Index::setCacheEnabled(bool enabled)
{
	m_tagList.setCacheEnabled(enabled);
}
QList<QPair<QString, QString> > Index::allGameTags(int gameId)
{
	QList<QPair<QString, QString> > list;
	QMap <int, QPair<quint8 , quint8 > >::const_iterator i = m_tagIndexPosition.constBegin();
	while (i != m_tagIndexPosition.constEnd()) {
		QString tagName = m_tagList.stringFromTag((Tag)i.key());
		int tagIndex = m_indexItems[gameId]->index(i.value().first, i.value().second);
		if (tagIndex >= 0) {
			QString tagValue = m_tagList[i.key()]->value(tagIndex);
			list.append(qMakePair(tagName, tagValue));
		}

		++i;
	}
	return list;
}
QBitArray Index::listContainingValue(Tag tag, const QString& value)
{
	Q_ASSERT((tag >= 0) && (tag <= TagLastTag));
	return ((StringTagValues*)m_tagList[tag])->listContainingValue(value);
}
QBitArray Index::listInRange(Tag tag, const QString& minValue, const QString& maxValue)
{
	Q_ASSERT((tag >= 0) && (tag <= TagLastTag));
	return ((StringTagValues*)m_tagList[tag])->listInRange(minValue, maxValue);
}
int Index::valueIndex(Tag tag, int gameId)
{
	return m_indexItems[gameId]->index(
		       m_tagIndexPosition[tag].first,
		       m_tagIndexPosition[tag].second);
}
Tag Index::tagFromString(const QString& tagName)
{
	return (Tag)m_tagList.tagFromString(tagName);
}
IndexItem* Index::item(int gameId)
{
	return m_indexItems[gameId];
}

void Index::loadGameHeaders(GameId id, Game& game)
{
	QList <QPair< QString, QString> > gameTags;
	gameTags = allGameTags(id);

	for (int i = 0; i < gameTags.count(); ++i)
		game.setTag(gameTags[i].first, gameTags[i].second);
}
