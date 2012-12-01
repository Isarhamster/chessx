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
#include <QHash>
#include <QMultiHash>

#include "index.h"

Index::Index()
{
    // Dummy Values in case a index is miscalculated
    m_tagNames[0]  = "?";
    m_tagValues[0] = "?";
}

Index::~Index()
{
}

GameId Index::add()
{
    int gameId = m_indexItems.count();
    m_indexItems.append(new IndexItem);
    m_validFlags.append(true);
    return gameId;
}

TagIndex Index::AddTagName(QString name)
{
    if (m_tagNameIndex.contains(name))
    {
        return m_tagNameIndex.value(name);
    }
    int n = m_tagNameIndex.size();
    m_tagNameIndex[name] = n;
    m_tagNames[n] = name;
    return n;
}

ValueIndex Index::AddTagValue(QString name)
{
    if (m_tagValueIndex.contains(name))
    {
        return m_tagValueIndex[name];
    }
    int n = m_tagValueIndex.size();
    m_tagValueIndex[name] = n;
    m_tagValues[n] = name;
    return n;
}

void Index::setTag(const QString& tagName, const QString& value, int gameId)
{
    TagIndex tagIndex = AddTagName(tagName);
    ValueIndex valueIndex = AddTagValue(value);

    if (tagName == TagNameWhite || tagName == TagNameBlack)
    {
        m_playerNames.insert(value);
    }
    if (m_indexItems.count() <= gameId)
    {
        add();
    }
    m_indexItems[gameId]->set(tagIndex, valueIndex);
    m_mapTagToIndexItems.insertMulti(tagIndex, gameId);
}

void Index::setValidFlag(const int& gameId, bool value)
{
    m_validFlags[gameId]=value;
}

bool Index::isValidFlag(const int& gameId) const
{
    return m_validFlags[gameId];
}

bool Index::write(QDataStream &out) const
{
    out << m_tagNames;
    out << m_tagValues;
    out << m_indexItems.count();
    for (int i=0; i<m_indexItems.count();++i)
    {
        m_indexItems[i]->write(out);
    }
    out << m_validFlags;

    bool extension = false;
    out << extension;

    return true;
}

bool Index::read(QDataStream &in)
{
    in >> m_tagNames;
    in >> m_tagValues;
    int itemCount;
    in >> itemCount;
    for (int i=0; i<itemCount;++i)
    {
        add();
        m_indexItems[i]->read(in);
    }
    in >> m_validFlags;

    bool extension;
    in >> extension;

    return true;
}

void Index::clear()
{
    for (int i = 0 ; i < m_indexItems.count() ; ++i)
    {
        delete m_indexItems[i];
	}
	m_indexItems.clear();
    m_tagNames.clear();
    m_tagValues.clear();
    m_validFlags.clear();
    m_mapTagToIndexItems.clear();
}

int Index::count() const
{
    return m_indexItems.count();
}

QBitArray Index::listContainingValue(const QString& tagName, const QString& value) const
{
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    ValueIndex valueIndex = m_tagValueIndex.value(value);

    QBitArray list(count(), false);
    for (int i = 0; i < count(); ++i) {
        list.setBit(i, m_indexItems[i]->valueIndex(tagIndex)==valueIndex);
    }
    return list;
}

QBitArray Index::listInRange(const QString& tagName, const QString& minValue, const QString& maxValue) const
{
    TagIndex tagIndex = m_tagNameIndex.value(tagName);

    QBitArray list(count(), false);
    for (int i = 0; i < count(); ++i) {
        QString value = tagValue(tagIndex,i);
        list.setBit(i, (minValue < value) && (value < maxValue));
    }
    return list;
}

QString Index::tagValue(TagIndex tagIndex, int gameId) const
{
    ValueIndex valueIndex = m_indexItems[gameId]->valueIndex(tagIndex);

    return m_tagValues.value(valueIndex);
}

QString Index::tagName(TagIndex tagIndex) const
{
    return m_tagNames.value(tagIndex);
}

QString Index::tagValueName(ValueIndex valueIndex) const
{
    return m_tagValues.value(valueIndex);
}

QString Index::tagValue(const QString& tagName, int gameId) const
{
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    return tagValue(tagIndex, gameId);
}

ValueIndex Index::valueIndexFromTag(const QString& tagName, int gameId) const
{
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    return m_indexItems[gameId]->valueIndex(tagIndex);
}

TagIndex Index::getTagIndex(const QString& value) const
{
    return m_tagNameIndex.value(value);
}

ValueIndex Index::getValueIndex(const QString& value) const
{
    return m_tagValueIndex.value(value);
}

IndexItem* Index::item(int gameId)
{
	return m_indexItems[gameId];
}

void Index::loadGameHeaders(GameId id, Game& game)
{
    game.clearTags();
    foreach (TagIndex tagIndex, m_indexItems[id]->getTagMapping().keys())
    {
        // qDebug() << "lGH>" << &game << " " << id << " " << tagName(tagIndex) << " " << tagValue(tagIndex, id);
        game.setTag(tagName(tagIndex), tagValue(tagIndex, id));
    }
}

QSet<QString>& Index::playerNames()
{
    QSet<ValueIndex> playerNameIndex;
    TagIndex tagIndex = getTagIndex(TagNameWhite);

    foreach(int gameId, m_mapTagToIndexItems.values(tagIndex))
    {
        playerNameIndex.insert(m_indexItems[gameId]->valueIndex(tagIndex));
    }

    tagIndex = getTagIndex(TagNameBlack);

    foreach(int gameId, m_mapTagToIndexItems.values(tagIndex))
    {
        playerNameIndex.insert(m_indexItems[gameId]->valueIndex(tagIndex));
    }

    foreach(ValueIndex valueIndex, playerNameIndex)
    {
        m_playerNames.insert(tagValueName(valueIndex));
    }

    return m_playerNames;
}
