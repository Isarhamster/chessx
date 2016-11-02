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
#include "tags.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

Index::Index()
{
    // Dummy Values in case a index is miscalculated
    m_tagNames[0]  = "?";
    m_tagValues[0] = "?";
}

Index::~Index()
{
    while (m_indexItems.count())
    {
        IndexItem* item = m_indexItems.first();
        delete item;
        m_indexItems.pop_front();
    }
}

GameId Index::add()
{
    GameId gameId = m_indexItems.count();
    m_indexItems.append(new IndexItem);
    return gameId;
}

TagIndex Index::AddTagName(QString name)
{
    if(m_tagNameIndex.contains(name))
    {
        return m_tagNameIndex.value(name);
    }
    TagIndex n = m_tagNameIndex.size();
    m_tagNameIndex[name] = n;
    m_tagNames[n] = name;
    return n;
}

ValueIndex Index::AddTagValue(QString name)
{
    if(m_tagValueIndex.contains(name))
    {
        return m_tagValueIndex[name];
    }
    ValueIndex n = m_tagValueIndex.size();
    m_tagValueIndex[name] = n;
    m_tagValues[n] = name;
    return n;
}

void Index::setTag(const QString& tagName, const QString& value, GameId gameId)
{
    TagIndex tagIndex = AddTagName(tagName);
    ValueIndex valueIndex = AddTagValue(value);

    if(m_indexItems.count() <= (int)gameId)
    {
        add();
    }
    m_indexItems[gameId]->set(tagIndex, valueIndex);
    m_mapTagToIndexItems.insertMulti(tagIndex, gameId);
}

void Index::removeTag(const QString& tagName, GameId gameId)
{
    if(m_tagNameIndex.contains(tagName))
    {
        TagIndex tagIndex = m_tagNameIndex.value(tagName);
        if((int)gameId < m_indexItems.count())
        {
            m_indexItems[gameId]->remove(tagIndex);
            m_mapTagToIndexItems.remove(tagIndex, gameId);
        }
    }
}

void Index::setValidFlag(GameId gameId, bool value)
{
    if (!value)
    {
        m_validFlags.insert(gameId);
    }
    else
    {
        m_validFlags.remove(gameId);
    }
}

bool Index::replaceTagValue(QStringList tags, const QString& newValue, const QString& oldValue)
{
    bool ok = false;
    foreach(QString t, tags)
    {
        if(m_tagNameIndex.contains(t))
        {
            ok = true;
            break;
        }
    }
    if(!ok || !m_tagValueIndex.contains(oldValue))
    {
        return false;
    }

    ValueIndex valueIndex = m_tagValueIndex[oldValue];

    if (m_tagValueIndex.contains(newValue))
    {
        ValueIndex newValueIndex = m_tagValueIndex[newValue];
        QList<TagIndex> tl;
        foreach (QString t, tags)
        {
            tl << getTagIndex(t);
        }
        foreach (IndexItem* i, m_indexItems)
        {
            i->replaceValue(tl, valueIndex, newValueIndex);
        }
        m_tagValues.remove(valueIndex);
    }
    else
    {
        m_tagValues[valueIndex] = newValue;
    }

    clearCache();
    calculateCache();
    return true;
}

bool Index::isValidFlag(GameId gameId) const
{
    return !m_validFlags.contains(gameId);
}

bool Index::write(QDataStream &out) const
{
    out << m_tagNames;
    out << m_tagValues;
    out << m_indexItems.count();
    for(int i = 0; i < m_indexItems.count(); ++i)
    {
        m_indexItems[i]->write(out);
    }
    out << m_validFlags;

    bool extension = false;
    out << extension;

    return true;
}

bool Index::read(QDataStream &in, volatile bool *breakFlag, short version)
{
    in >> m_tagNames;
    in >> m_tagValues;
    int itemCount;
    in >> itemCount;

    int countDiff = itemCount / 100;
    int nextDiff = countDiff;
    int percentDone = 0;

    for(int i = 0; i < itemCount; ++i)
    {
        if(*breakFlag)
        {
            return false;
        }
        add();
        m_indexItems[i]->read(in);
        if(i >= nextDiff)
        {
            nextDiff += countDiff;
            emit progress(++percentDone);
        }
    }
    if (version <= VERSION_INDEX_1_2)
    {
        QList<bool> validFlags;
        in >> validFlags;
        for (int i=0; i<validFlags.size(); ++i)
        {
            setValidFlag(i, validFlags[i]);
        }
    }
    else
    {
        in >> m_validFlags;
    }
    bool extension;
    in >> extension;

    calculateCache(breakFlag);

    return !(*breakFlag);
}

void Index::clearCache()
{
    m_tagNameIndex.clear();
    m_tagValueIndex.clear();
    m_mapTagToIndexItems.clear();
}

void Index::calculateCache(volatile bool* breakFlag)
{
    calculateReverseMaps(breakFlag);
    calculateTagMap(breakFlag);
}

void Index::calculateReverseMaps(volatile bool* breakFlag)
{
    if(m_tagNameIndex.isEmpty())
    {
        foreach(TagIndex tagIndex, m_tagNames.keys())
        {
            if(breakFlag && *breakFlag)
            {
                return;
            }
            m_tagNameIndex.insert(m_tagNames.value(tagIndex), tagIndex);
        }
    }
    if(m_tagValueIndex.isEmpty())
    {
        foreach(ValueIndex valueIndex, m_tagValues.keys())
        {
            if(breakFlag && *breakFlag)
            {
                return;
            }
            m_tagValueIndex.insert(m_tagValues.value(valueIndex), valueIndex);
        }
    }
}

void Index::calculateTagMap(volatile bool *breakFlag)
{
    if(m_mapTagToIndexItems.isEmpty())
    {
        foreach(TagIndex tagIndex, m_tagNames.keys())
        {
            for(GameId gameId = 0; gameId < (GameId)m_indexItems.size(); ++gameId)
            {
                if(breakFlag && *breakFlag)
                {
                    return;
                }
                if(indexItemHasTag(tagIndex, gameId))
                {
                    m_mapTagToIndexItems.insertMulti(tagIndex, gameId);
                }
            }
        }
    }
}

void Index::clear()
{
    for(int i = 0 ; i < m_indexItems.count() ; ++i)
    {
        delete m_indexItems[i];
    }
    m_indexItems.clear();
    m_tagNames.clear();
    m_tagNameIndex.clear();
    m_tagValues.clear();
    m_tagValueIndex.clear();
    m_deletedGames.clear();
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
    for(int i = 0; i < count(); ++i)
    {
        list.setBit(i, m_indexItems[i]->valueIndex(tagIndex) == valueIndex);
    }
    return list;
}

QBitArray Index::listInSet(const QString& tagName, const QSet<QString>& set, bool partial) const
{
    TagIndex tagIndex = m_tagNameIndex.value(tagName);

    QBitArray list(count(), false);
    if (partial)
    {
        for(int i = 0; i < count(); ++i)
        {
            QString value = tagValue(tagIndex, i);
            bool b = false;
            foreach(QString s, set)
            {
                if (value.contains(s, Qt::CaseInsensitive))
                {
                    b = true;
                    break;
                }
            }
            list.setBit(i, b);
        }
    }
    else
    {
        for(int i = 0; i < count(); ++i)
        {
            QString value = tagValue(tagIndex, i);
            list.setBit(i, set.contains(value));
        }
    }
    return list;
}

QBitArray Index::listInRange(const QString& tagName, const QString& minValue, const QString& maxValue) const
{
    TagIndex tagIndex = m_tagNameIndex.value(tagName);

    QBitArray list(count(), false);
    for(int i = 0; i < count(); ++i)
    {
        QString value = tagValue(tagIndex, i);
        list.setBit(i, (minValue <= value) && (value <= maxValue));
    }
    return list;
}

QBitArray Index::listInRange(const QString &tagName, int minValue, int maxValue) const
{
    TagIndex tagIndex = m_tagNameIndex.value(tagName);

    QBitArray list(count(), false);
    for(int i = 0; i < count(); ++i)
    {
        int value = tagValue(tagIndex, i).toInt();
        list.setBit(i, (minValue <= value) && (value <= maxValue));
    }
    return list;
}

QBitArray Index::listPartialValue(const QString& tagName, const QString& value) const
{
    TagIndex tagIndex = m_tagNameIndex.value(tagName);

    QBitArray list(count(), false);
    for(int i = 0; i < count(); ++i)
    {
        QString gameValue = tagValue(tagIndex, i);
        list.setBit(i, gameValue.contains(value, Qt::CaseInsensitive));
    }
    return list;
}

QString Index::tagValue(TagIndex tagIndex, GameId gameId) const
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

QString Index::tagValue(const QString& tagName, GameId gameId) const
{
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    return tagValue(tagIndex, gameId);
}

ValueIndex Index::valueIndexFromTag(const QString& tagName, GameId gameId) const
{
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    return m_indexItems[gameId]->valueIndex(tagIndex);
}

bool Index::indexItemHasTag(TagIndex tagIndex, GameId gameId) const
{
    return m_indexItems[gameId]->hasTagIndex(tagIndex);
}

inline ValueIndex Index::valueIndexFromIndex(TagIndex tagIndex, GameId gameId) const
{
    return m_indexItems[gameId]->valueIndex(tagIndex);
}

TagIndex Index::getTagIndex(const QString& value) const
{
    if(m_tagNameIndex.contains(value))
    {
        return m_tagNameIndex.value(value);
    }
    return TagNoIndex;
}

ValueIndex Index::getValueIndex(const QString& value) const
{
    return m_tagValueIndex.value(value);
}

const IndexItem* Index::item(GameId gameId) const
{
    return m_indexItems[gameId];
}

IndexItem* Index::item(GameId gameId)
{
    return m_indexItems[gameId];
}

unsigned int Index::hashIndexItem(GameId gameId) const
{
    // return m_indexItems[gameId]->hash();
    return valueIndexFromTag(TagNameWhite, gameId);
}

bool Index::isIndexItemEqual(GameId i, GameId j) const
{
    return (item(i)->isEqual(*item(j)));
}

void Index::loadGameHeaders(GameId id, Game& game) const
{
    game.clearTags();
    foreach(TagIndex tagIndex, m_indexItems[id]->getTagIndices())
    {
        // qDebug() << "lGH>" << &game << " " << id << " " << tagName(tagIndex) << " " << tagValue(tagIndex, id);
        game.setTag(tagName(tagIndex), tagValue(tagIndex, id));
    }
}

void Index::loadGameHeader(GameId id, Game& game, const QString &tag) const
{
    TagIndex tagIndex = getTagIndex(tag);
    game.setTag(tagName(tagIndex), tagValue(tagIndex, id));
}

QStringList Index::playerNames() const
{
    QStringList allPlayerNames;
    QSet<ValueIndex> playerNameIndex;

    TagIndex tagIndex = getTagIndex(TagNameWhite);
    if(tagIndex != TagNoIndex)
    {
        foreach(GameId gameId, m_mapTagToIndexItems.values(tagIndex))
        {
            playerNameIndex.insert(m_indexItems[gameId]->valueIndex(tagIndex));
        }
    }

    tagIndex = getTagIndex(TagNameBlack);
    if(tagIndex != TagNoIndex)
    {
        foreach(GameId gameId, m_mapTagToIndexItems.values(tagIndex))
        {
            playerNameIndex.insert(m_indexItems[gameId]->valueIndex(tagIndex));
        }
    }

    foreach(ValueIndex valueIndex, playerNameIndex)
    {
        allPlayerNames.append(tagValueName(valueIndex));
    }

    return allPlayerNames;
}

QStringList Index::tagValues(const QString& tagName) const
{
    QStringList allTagNames;
    QSet<ValueIndex> tagNameIndex;
    TagIndex tagIndex = getTagIndex(tagName);

    if(tagIndex != TagNoIndex)
    {
        foreach(GameId gameId, m_mapTagToIndexItems.values(tagIndex))
        {
            tagNameIndex.insert(m_indexItems[gameId]->valueIndex(tagIndex));
        }

        foreach(ValueIndex valueIndex, tagNameIndex)
        {
            allTagNames.append(tagValueName(valueIndex));
        }
    }

    return allTagNames;
}

bool Index::deleted(GameId gameId) const
{
    return m_deletedGames.contains(gameId);
}

void Index::setDeleted(GameId gameId, bool df)
{
    if (df)
    {
        m_deletedGames.insert(gameId);
    }
    else
    {
        m_deletedGames.remove(gameId);
    }
}
