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
#include <QRegularExpression>
#include <QVector>

#include "index.h"
#include "tags.h"

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

IndexX::IndexX() : m_mutex(QReadWriteLock::Recursive)
{
    // Dummy Values in case a index is miscalculated
    init();
}

IndexX::~IndexX()
{
}

GameId IndexX::add()
{
    QWriteLocker m(&m_mutex);
    GameId gameId = m_indexItems.count();
    m_indexItems.insert(gameId, IndexItem());
    return gameId;
}

TagIndex IndexX::AddTagName(const QString& name)
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

ValueIndex IndexX::AddTagValue(QString name)
{
    ValueIndex n = qHash(name);
    if (m_tagValues.contains(n))
    {
        if (m_tagValues[n] == name)
        {
            return n;
        }
        name.append(QChar(0));
        QString prelim;
        int i = 0;
        do {
            prelim = name + QString::number(i++);
            n = qHash(prelim);
            if (m_tagValues.contains(n))
            {
                if (m_tagValues[n] == prelim)
                {
                    return n;
                }
            }
        } while(m_tagValues.contains(n));
        name = prelim;
    }
    m_tagValues[n] = name;
    return n;
}

void IndexX::setTag(const QString& tagName, const QString& value, GameId gameId)
{
	QWriteLocker m(&m_mutex); // PERF 10s aus 30s (aus 115s Gesamtdatei) 
	setTag_nolock(tagName, value, gameId);
}

void IndexX::setTag_nolock(const QString& tagName, const QString& value, GameId gameId)
{
	TagIndex tagIndex = AddTagName(tagName);
	ValueIndex valueIndex = AddTagValue(value);

	if (m_indexItems.count() <= (int)gameId)
	{
		(void) add();
	}
	m_indexItems[gameId].set(tagIndex, valueIndex);
}

void IndexX::removeTag(const QString& tagName, GameId gameId)
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

bool IndexX::hasTag(const QString& tagName, GameId gameId) const
{
    QReadLocker m(&m_mutex);
    if(m_tagNameIndex.contains(tagName))
    {
        TagIndex tagIndex = m_tagNameIndex.value(tagName);
        if((int)gameId < m_indexItems.count())
        {
            return m_indexItems[gameId].hasTagIndex(tagIndex);
        }
    }
    return false;
}

void IndexX::setValidFlag(GameId gameId, bool value)
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

bool IndexX::replaceTagValue(const QStringList& tags, const QString& newValue, const QString& oldValue)
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
    ValueIndex valueIndex = getValueIndex(oldValue);
    if(!ok || !m_tagValues.contains(valueIndex))
    {
        return false;
    }
    ValueIndex newIndex = getValueIndex(newValue);

    if (!m_tagValues.contains(newIndex))
    {
        (void) AddTagValue(newValue); // Adds newIndex-newValue pair, return must be newIndex
    }

    QList<TagIndex> tl;
    foreach (QString t, tags)
    {
        tl << getTagIndex(t);
    }

     for (auto i = m_indexItems.begin(); i != m_indexItems.end(); ++i)
    {
        i->replaceValue(tl, valueIndex, newIndex);
    }

    m_tagValues.remove(valueIndex);
    return true;
}

bool IndexX::isValidFlag(GameId gameId) const
{
    QReadLocker m(&m_mutex);
    return !m_validFlags.contains(gameId);
}

bool IndexX::write(QDataStream &out) const
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

void IndexX::reserve(quint32 estimation)
{
    m_tagValues.reserve(estimation+16);
}

void IndexX::squeeze()
{
    m_tagValues.squeeze();
}

bool IndexX::read(QDataStream &in, volatile bool *breakFlag, short version)
{
    Q_UNUSED(version);

    QWriteLocker m(&m_mutex);

    in >> m_tagNames;
    in >> m_tagValues;
	in >> m_indexItems;
    in >> m_validFlags;
    
	bool extension;
    in >> extension;

    m_tagNameIndex.clear();

    calculateCache(breakFlag);

    return !(*breakFlag);
}

void IndexX::clearCache()
{
    QWriteLocker m(&m_mutex);
    m_tagNameIndex.clear();
}

void IndexX::calculateCache(volatile bool* breakFlag)
{
    calculateReverseMaps(breakFlag);
}

void IndexX::calculateReverseMaps(volatile bool* breakFlag)
{
    if(m_tagNameIndex.isEmpty())
    {
        for (auto it = m_tagNames.cbegin(); it != m_tagNames.cend(); ++it)
        {
            if(breakFlag && *breakFlag)
            {
                return;
            }
            m_tagNameIndex.insert(it.value(), it.key());
        }
    }
}

void IndexX::init()
{
    AddTagName("?");
    AddTagValue("?");
}

void IndexX::clear()
{
    QWriteLocker m(&m_mutex);
    m_indexItems.clear();
    m_tagNames.clear();
    m_tagNameIndex.clear();
    m_tagValues.clear();
    m_deletedGames.clear();
    m_validFlags.clear();
    init(); // Just to make sure that the index can be used after clearing
}

int IndexX::count() const
{
    return m_indexItems.count();
}

QBitArray IndexX::listInSet(const QString& tagName, const QSet<QString>& set) const
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

QBitArray IndexX::listInRange(const QString& tagName, const QString& minValue, const QString& maxValue) const
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

QBitArray IndexX::listInRange(const QString &tagName, int minValue, int maxValue) const
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

QBitArray IndexX::listPartialValue(const QString& tagName, QString value) const
{
    QReadLocker m(&m_mutex);
    value.replace("-","\\-"); // Avoid - to become range
    value.replace("(","\\("); // Avoid () to become regex
    value.replace(")","\\)");
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    QRegularExpression re(value);
    re.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    QBitArray list(count(), false);
    for(int i = 0; i < count(); ++i)
    {
        QString gameValue = tagValue(tagIndex, i);
        list.setBit(i, gameValue.contains(re));
    }
    return list;
}

QString IndexX::tagValue_byIndex(TagIndex tagIndex, GameId gameId) const
{
    QReadLocker m(&m_mutex);

    if (m_indexItems.length() <= gameId) return QString();
    ValueIndex valueIndex = m_indexItems[gameId].valueIndex(tagIndex);

    return tagValueName(valueIndex);
}

QString IndexX::tagValue(TagIndex tagIndex, GameId gameId) const
{
    if (m_indexItems.length() <= gameId) return QString();
    ValueIndex valueIndex = m_indexItems[gameId].valueIndex(tagIndex);

    return tagValueName(valueIndex);
}

QString IndexX::tagName(TagIndex tagIndex) const
{
    return m_tagNames.value(tagIndex);
}

QString IndexX::tagValueName(ValueIndex valueIndex) const
{
    QString r = m_tagValues.value(valueIndex);
    return r.section(QChar(0),0,0);
}

QString IndexX::tagValue(const QString& tagName, GameId gameId) const
{
    QReadLocker m(&m_mutex);
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    return tagValue(tagIndex, gameId);
}

ValueIndex IndexX::valueIndexFromTag(const QString& tagName, GameId gameId) const
{
    QReadLocker m(&m_mutex);
    TagIndex tagIndex = m_tagNameIndex.value(tagName);
    return valueIndexFromIndex(tagIndex, gameId);
}

bool IndexX::indexItemHasTag(TagIndex tagIndex, GameId gameId) const
{
    if (m_indexItems.length() <= gameId) return false;
    return m_indexItems[gameId].hasTagIndex(tagIndex);
}

inline ValueIndex IndexX::valueIndexFromIndex(TagIndex tagIndex, GameId gameId) const
{
    if (m_indexItems.length() <= gameId) return ValueNoIndex;
    return m_indexItems[gameId].valueIndex(tagIndex);
}

TagIndex IndexX::getTagIndex(const QString& value) const
{
    if(m_tagNameIndex.contains(value))
    {
        return m_tagNameIndex.value(value);
    }
    return TagNoIndex;
}

ValueIndex IndexX::getValueIndex(QString name) const
{
    ValueIndex n = qHash(name);

    if (m_tagValues.contains(n))
    {
        if (m_tagValues[n] == name)
        {
            return n;
        }
        name.append(QChar(0));
        QString prelim;
        int i = 0;
        do {
            prelim = name + QString::number(i++);
            n = qHash(prelim);
            if (m_tagValues.contains(n))
            {
                if (m_tagValues[n] == prelim)
                {
                    return n;
                }
            }
        } while(m_tagValues.contains(n));
    }

    return n;
}

unsigned int IndexX::hashIndexItem(GameId gameId) const
{
    return valueIndexFromTag(TagNameWhite, gameId);
}

bool IndexX::isIndexItemEqual(GameId i, GameId j) const
{
    QReadLocker m(&m_mutex);
    const IndexItem& iItem = m_indexItems.value(i);
    const IndexItem& jItem = m_indexItems.value(j);
    return (iItem.isEqual(jItem));
}

void IndexX::loadGameHeaders(GameId id, GameX& game) const
{
    QReadLocker m(&m_mutex);

    game.clearTags();
    foreach(TagIndex tagIndex, m_indexItems[id].getTagIndices())
    {
        // qDebug() << "lGH>" << &game << " " << id << " " << tagName(tagIndex) << " " << tagValue(tagIndex, id);
        game.setTag(tagName(tagIndex), tagValue(tagIndex, id));
    }
}

void IndexX::loadGameHeader(GameId id, GameX& game, const QString &tag) const
{
    QReadLocker m(&m_mutex);

    TagIndex tagIndex = getTagIndex(tag);
    game.setTag(tagName(tagIndex), tagValue(tagIndex, id));
}

QStringList IndexX::playerNames() const
{
    QReadLocker m(&m_mutex);

    QStringList allPlayerNames;
    QSet<ValueIndex> playerNameIndex;

    TagIndex tagIndex = getTagIndex(TagNameWhite);
    if(tagIndex != TagNoIndex)
    {
        QVector<IndexItem>::const_iterator i;
		for (i = m_indexItems.constBegin(); i != m_indexItems.constEnd(); ++i)
		{
            playerNameIndex.insert(i->valueIndex(tagIndex));
		}
    }

    tagIndex = getTagIndex(TagNameBlack);
    if(tagIndex != TagNoIndex)
    {
        QVector<IndexItem>::const_iterator i;
		for (i = m_indexItems.constBegin(); i != m_indexItems.constEnd(); ++i)
		{
            playerNameIndex.insert(i->valueIndex(tagIndex));
		}
	}

    foreach(ValueIndex valueIndex, playerNameIndex)
    {
        allPlayerNames.append(tagValueName(valueIndex));
    }

    return allPlayerNames;
}

QSet<ValueIndex> IndexX::tagValueSet(const QString& tagName) const
{
	QReadLocker m(&m_mutex);

	QSet<ValueIndex> tagNameIndex;
	TagIndex tagIndex = getTagIndex(tagName);

	if (tagIndex != TagNoIndex)
	{
        QVector<IndexItem>::const_iterator i;
		for (i = m_indexItems.constBegin(); i != m_indexItems.constEnd(); ++i)
		{
            tagNameIndex.insert(i->valueIndex(tagIndex));
		}
	}
	return tagNameIndex;
}

QStringList IndexX::tagNames() const
{
    return m_tagNameIndex.keys();
}

QStringList IndexX::tagValues(const QString& tagName) const
{
	QStringList allTagNames;
	QSet<ValueIndex> set = tagValueSet(tagName);
    foreach(ValueIndex valueIndex, set)
    {
        allTagNames.append(tagValueName(valueIndex));
    }
    return allTagNames;
}

bool IndexX::deleted(GameId gameId) const
{
    return m_deletedGames.contains(gameId);
}

void IndexX::setDeleted(GameId gameId, bool df)
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
