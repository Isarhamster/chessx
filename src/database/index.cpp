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
#include <QWriteLocker>
#include <QReadLocker>

#include "index.h"
#include "tags.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

Index::Index() : m_mutex(QReadWriteLock::Recursive)
{
    // Dummy Values in case a index is miscalculated
    init();
}

Index::~Index()
{
}

GameId Index::add()
{
    QWriteLocker m(&m_mutex);
    GameId gameId = m_indexItems.count();
    m_indexItems.insert(gameId, IndexItem());
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
	QWriteLocker m(&m_mutex); // PERF 10s aus 30s (aus 115s Gesamtdatei) 
	setTag_nolock(tagName, value, gameId);
}

void Index::setTag_nolock(const QString& tagName, const QString& value, GameId gameId)
{
	TagIndex tagIndex = AddTagName(tagName);
	ValueIndex valueIndex = AddTagValue(value);

	if (m_indexItems.count() <= (int)gameId)
	{
		(void) add();
	}
	m_indexItems[gameId].set(tagIndex, valueIndex);
}

void Index::removeTag(const QString& tagName, GameId gameId)
{
    QWriteLocker m(&m_mutex);
    if(m_tagNameIndex.contains(tagName))
    {
        TagIndex tagIndex = m_tagNameIndex.value(tagName);
        if((int)gameId < m_indexItems.count())
        {
            m_indexItems[gameId].remove(tagIndex);
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
    QWriteLocker m(&m_mutex);

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

		QHash<GameId, IndexItem>::iterator i;
		for (i = m_indexItems.begin(); i != m_indexItems.end(); ++i)
        {
            i.value().replaceValue(tl, valueIndex, newValueIndex);
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
    QReadLocker m(&m_mutex);
    return !m_validFlags.contains(gameId);
}

bool Index::write(QDataStream &out) const
{
    QReadLocker m(&m_mutex);

    out << m_tagNames;
    out << m_tagValues;
	out << m_indexItems;
    out << m_validFlags;

    bool extension = false;
    out << extension;

    return true;
}

bool Index::read(QDataStream &in, volatile bool *breakFlag, short version)
{
    QWriteLocker m(&m_mutex);

    in >> m_tagNames;
    in >> m_tagValues;
	in >> m_indexItems;
    in >> m_validFlags;
    
	bool extension;
    in >> extension;

    m_tagNameIndex.clear();
    m_tagValueIndex.clear();

    calculateCache(breakFlag);

    return !(*breakFlag);
}

void Index::clearCache()
{
    QWriteLocker m(&m_mutex);
    m_tagNameIndex.clear();
    m_tagValueIndex.clear();
}

void Index::calculateCache(volatile bool* breakFlag)
{
    calculateReverseMaps(breakFlag);
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

void Index::init()
{
    AddTagName("?");
    AddTagValue("?");
}

void Index::clear()
{
    QWriteLocker m(&m_mutex);
    m_indexItems.clear();
    m_tagNames.clear();
    m_tagNameIndex.clear();
    m_tagValues.clear();
    m_tagValueIndex.clear();
    m_deletedGames.clear();
    m_validFlags.clear();
}

int Index::count() const
{
    return m_indexItems.count();
}

QBitArray Index::listInSet(const QString& tagName, const QSet<QString>& set) const
{
    QReadLocker m(&m_mutex);

    TagIndex tagIndex = m_tagNameIndex.value(tagName);

    QBitArray list(count(), false);
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
    return list;
}

QBitArray Index::listInRange(const QString& tagName, const QString& minValue, const QString& maxValue) const
{
    QReadLocker m(&m_mutex);

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
    QReadLocker m(&m_mutex);

    TagIndex tagIndex = m_tagNameIndex.value(tagName);

    QBitArray list(count(), false);
    for(int i = 0; i < count(); ++i)
    {
        int value = tagValue(tagIndex, i).toInt();
        list.setBit(i, (minValue <= value) && (value <= maxValue));
    }
    return list;
}

QBitArray Index::listPartialValue(const QString& tagName, QString value) const
{
    QReadLocker m(&m_mutex);
    value.replace("-","\\-"); // Avoid - to become range
    value.replace("(","\\("); // Avoid () to become regex
    value.replace(")","\\)");
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    QRegExp re(value);
    re.setCaseSensitivity(Qt::CaseInsensitive);
    QBitArray list(count(), false);
    for(int i = 0; i < count(); ++i)
    {
        QString gameValue = tagValue(tagIndex, i);
        list.setBit(i, gameValue.contains(re));
    }
    return list;
}

QString Index::tagValue(TagIndex tagIndex, GameId gameId) const
{
    ValueIndex valueIndex = m_indexItems[gameId].valueIndex(tagIndex);

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
    QReadLocker m(&m_mutex);
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    return tagValue(tagIndex, gameId);
}

ValueIndex Index::valueIndexFromTag(const QString& tagName, GameId gameId) const
{
    QReadLocker m(&m_mutex);
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    return valueIndexFromIndex(tagIndex, gameId);
}

bool Index::indexItemHasTag(TagIndex tagIndex, GameId gameId) const
{
    return m_indexItems[gameId].hasTagIndex(tagIndex);
}

inline ValueIndex Index::valueIndexFromIndex(TagIndex tagIndex, GameId gameId) const
{
    return m_indexItems[gameId].valueIndex(tagIndex);
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
    QReadLocker m(&m_mutex);
    return m_tagValueIndex.value(value);
}

unsigned int Index::hashIndexItem(GameId gameId) const
{
    return valueIndexFromTag(TagNameWhite, gameId);
}

bool Index::isIndexItemEqual(GameId i, GameId j) const
{
    QReadLocker m(&m_mutex);
	const IndexItem* iItem = &m_indexItems[i];
	const IndexItem* jItem = &m_indexItems[j];
    return (iItem->isEqual(*jItem));
}

void Index::loadGameHeaders(GameId id, Game& game) const
{
    QReadLocker m(&m_mutex);

    game.clearTags();
    foreach(TagIndex tagIndex, m_indexItems[id].getTagIndices())
    {
        // qDebug() << "lGH>" << &game << " " << id << " " << tagName(tagIndex) << " " << tagValue(tagIndex, id);
        game.setTag(tagName(tagIndex), tagValue(tagIndex, id));
    }
}

void Index::loadGameHeader(GameId id, Game& game, const QString &tag) const
{
    QReadLocker m(&m_mutex);

    TagIndex tagIndex = getTagIndex(tag);
    game.setTag(tagName(tagIndex), tagValue(tagIndex, id));
}

QStringList Index::playerNames() const
{
    QReadLocker m(&m_mutex);

    QStringList allPlayerNames;
    QSet<ValueIndex> playerNameIndex;

    TagIndex tagIndex = getTagIndex(TagNameWhite);
    if(tagIndex != TagNoIndex)
    {
		QHash<GameId, IndexItem>::const_iterator i;
		for (i = m_indexItems.constBegin(); i != m_indexItems.constEnd(); ++i)
		{
			playerNameIndex.insert(i.value().valueIndex(tagIndex));
		}
    }

    tagIndex = getTagIndex(TagNameBlack);
    if(tagIndex != TagNoIndex)
    {
		QHash<GameId, IndexItem>::const_iterator i;
		for (i = m_indexItems.constBegin(); i != m_indexItems.constEnd(); ++i)
		{
			playerNameIndex.insert(i.value().valueIndex(tagIndex));
		}
	}

    foreach(ValueIndex valueIndex, playerNameIndex)
    {
        allPlayerNames.append(tagValueName(valueIndex));
    }

    return allPlayerNames;
}

QSet<ValueIndex> Index::tagValueSet(const QString& tagName) const
{
	QReadLocker m(&m_mutex);

	QSet<ValueIndex> tagNameIndex;
	TagIndex tagIndex = getTagIndex(tagName);

	if (tagIndex != TagNoIndex)
	{
		QHash<GameId, IndexItem>::const_iterator i;
		for (i = m_indexItems.constBegin(); i != m_indexItems.constEnd(); ++i)
		{
			tagNameIndex.insert(i.value().valueIndex(tagIndex));
		}
	}
	return tagNameIndex;
}

QStringList Index::tagValues(const QString& tagName) const
{
	QStringList allTagNames;
	QSet<ValueIndex> set = tagValueSet(tagName);
    foreach(ValueIndex valueIndex, set)
    {
        allTagNames.append(tagValueName(valueIndex));
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
