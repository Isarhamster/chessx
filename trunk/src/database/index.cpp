/***************************************************************************
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>                *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QtDebug>
#include <QFile>
#include <QDataStream>

#include "index.h"

#define INDEX_ITEM_BUFFER_SIZE 512

const int Index::defaultIndexItemSize = 32;

Index::Index()
{
    m_tagIndexSize = defaultIndexItemSize;

    setTagIndexPosition(TagEvent, 0, 4);
    setTagIndexPosition(TagSite, 4, 4);
    setTagIndexPosition(TagDate, 8, 2);
    setTagIndexPosition(TagRound, 10, 2);
    setTagIndexPosition(TagWhite, 12, 4);
    setTagIndexPosition(TagBlack, 16, 4);
    setTagIndexPosition(TagFEN, 20, 4);
    setTagIndexPosition(TagWhiteElo, 24, 2);
    setTagIndexPosition(TagBlackElo, 26, 2);
    setTagIndexPosition(TagECO, 28, 2);
    setTagIndexPosition(TagResult, 30, 2);
    createIndexItems();
	reallocateIndexItems();

	m_nbUsedIndexItems = 0;
}

Index::~Index()
{
    clear();
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
	if (gameId >= m_indexItems.size()) {
		for (int i = m_indexItems.size(); i <= gameId; ++i)
		{
	  	  add();
		}
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
    m_tagIndexSize = m_indexItems[gameId]->allocate(m_tagIndexSize);
    m_deleteFlags.append(false);
    m_validFlags.append(true);
	return gameId;
}

// only a dummy implementation at the moment
GameId Index::add(const Game&)
{
#ifdef __GNUG__
#warning Just a dummy implementation of Index::add()
#else // VisualC++
#pragma message("Just a dummy implementation of Index::add()")
#endif
	++m_nbUsedIndexItems;
	return add();
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

TagIndex Index::add(const IndexItem& item)
{
	return add(new IndexItem(item));
}

TagIndex Index::add(IndexItem* item)
{
    if (m_nbUsedIndexItems >= m_indexItems.count())
    {
        createIndexItems();
    }
	m_indexItems[m_nbUsedIndexItems] = item;
	return m_nbUsedIndexItems++;
}

void Index::replace(const IndexItem& item, const int& gameId)
{
	return replace(new IndexItem(item),gameId);
}

void Index::replace(IndexItem* item, const int& gameId)
{
	delete m_indexItems[gameId];
	m_indexItems[gameId]=item;
}

void Index::reallocateIndexItems(bool clear)
{
	calculateIndexSize();

    for (int j = 0; j < m_indexItems.count(); ++j)
    {
        m_indexItems[j]->allocate(m_tagIndexSize, clear);
    }
}

QString Index::tagValue(Tag tag, int gameId)
{
	return m_tagList[tag]->value(m_indexItems[gameId]->index(
					     m_tagIndexPosition[tag].first,
					     m_tagIndexPosition[tag].second));
}

bool Index::deleteFlag(const int& gameId) const
{
	return m_deleteFlags[gameId];
}

void Index::setDeleteFlag(const int& gameId, const bool& df)
{
	m_deleteFlags[gameId]=df;
}

void Index::setValidFlag(const int& gameId, bool value)
{
    m_validFlags[gameId]=value;
}

bool Index::isValidFlag(const int& gameId) const
{
    return m_validFlags[gameId];
}

bool Index::toggleDeleteFlag(const int& gameId)
{
	return m_deleteFlags[gameId]=!m_deleteFlags[gameId];
}

bool Index::write(QDataStream &out)
{
	// Write all the tag values
	m_tagList.write(out);

	// Write the individual index items
	out << m_indexItems.count();
    out << m_tagIndexSize;

    for (int i = 0 ; i < m_indexItems.count() ; ++i)
    {
		m_indexItems[i]->write(out);
	}

    out << m_tagIndexPosition;

    return true;
}

bool Index::read(QDataStream &in)
{
	// Read all the tag values
	m_tagList.read(in);

	// Read the individual index items
	int indexItemCount;
	in >> indexItemCount;
    in >> m_tagIndexSize;
	for (int i = 0 ; i < indexItemCount ; ++i) {
		add();
		m_indexItems[i]->read(in);
	}

    m_tagIndexPosition.clear();
    in >> m_tagIndexPosition;

    m_nbUsedIndexItems = m_indexItems.count();

    return true;
}

void Index::clear()
{
    for (int i = 0 ; i < m_indexItems.count() ; ++i)
    {
		delete m_indexItems[i];
	}
	m_indexItems.clear();
	m_tagList.clear();
    m_deleteFlags.clear();
    m_validFlags.clear();
}

void Index::setCacheEnabled(bool enabled)
{
	m_tagList.setCacheEnabled(enabled);
}

void Index::allGameTags(int gameId, QList<QPair<QString, QString> >& list)
{
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

int Index::valueToIndex(Tag tag, const QString& value) const
{
	return m_tagList[tag]->indexOf(value);
}

QString Index::indexToValue(Tag tag, int value) const
{
	return m_tagList[tag]->value(value);
}

QString Index::gameTagValue(Tag tag, int gameId) const
{
	return indexToValue(tag, gameTagIndex(tag, gameId));
}

int Index::gameTagIndex(Tag tag, int gameId) const
{
	return m_indexItems[gameId]->index(m_tagIndexPosition[tag].first,
						m_tagIndexPosition[tag].second);
}

Tag Index::tagFromString(const QString& tagName)
{
	return Tag(m_tagList.tagFromString(tagName));
}

IndexItem* Index::item(int gameId)
{
	return m_indexItems[gameId];
}

void Index::loadGameHeaders(GameId id, Game& game)
{
	QList <QPair< QString, QString> > gameTags;
	allGameTags(id, gameTags);

	for (int i = 0; i < gameTags.count(); ++i)
		game.setTag(gameTags[i].first, gameTags[i].second);
}

TagValues* Index::tagValues(Tag tag)
{
	return m_tagList[tag];
}

