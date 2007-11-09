/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 20/02/2007
    copyright            : (C) 2007 Marius Roets
                           <saidinwielder@users.sourceforge.net>
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
#include "taglist.h"
#include "common.h"

void TagList::createBasicTagMap()
{
	BasicTagMap.clear();
	// Choose some sufficiently large value, there should not be more
	// tags created than this value.
	for (int i = 0; i < 200; ++i) {
		BasicTagMap[(Tag)i] = (Tag)i;
	}
	BasicTagMap[TagWhite] = TagPlayerName;
	BasicTagMap[TagBlack] = TagPlayerName;
	BasicTagMap[TagWhiteTitle] = TagPlayerTitle;
	BasicTagMap[TagBlackTitle] = TagPlayerTitle;
	BasicTagMap[TagWhiteElo] = TagPlayerElo;
	BasicTagMap[TagBlackElo] = TagPlayerElo;
	BasicTagMap[TagWhiteUSCF] = TagPlayerUSCF;
	BasicTagMap[TagBlackUSCF] = TagPlayerUSCF;
	BasicTagMap[TagWhiteNA] = TagPlayerNA;
	BasicTagMap[TagBlackNA] = TagPlayerNA;
	BasicTagMap[TagWhiteType] = TagPlayerType;
	BasicTagMap[TagBlackType] = TagPlayerType;
}
TagList::TagList()
{
	TagList::createBasicTagMap();
	m_list.clear();

	for (int i = 0 ; i < TagLastTag; ++i) {
		m_tagNameToInt.insert(TagNames[i], i);
		/* Here is something that could be worked on later. It is
		 * strictly speaking not necessary to create a value list
		 * for every possible tag. However the Black/White -> PlayerName
		 * conversion is much simpler this way. The downside is that
		 * between 12 and ? empty and unused objects are created
		 * per database. Currently this seems like a small price to pay. */
		// rico: I do not think this is a big issue. Furthermore it makes
		// access to the tags a bit easier. The implementation of chessxdatabase
		// currently relies on this handling of m_list.
		m_list.append(NULL);
	}
	// I comment these out, because I am not sure how to handle the integers currently.
	// It just means that everything is handled as strings.
	/*    m_list[TagPlayerElo] = new TagValues;
	   m_list[TagPlayerUSCF] = new TagValues;
	   m_list[TagPlyCount] = new TagValues; */
	for (int i = 0; i < TagWhite ; ++i) {
		if (!m_list.at(i)) {
			m_list[i] = static_cast<TagValues*>(new StringTagValues);
		}

	}
	m_cacheEnabled = false;

}
TagList::~TagList()
{
	clear();
}
void TagList::setCacheEnabled(bool enabled)
{
	m_cacheEnabled = enabled;
	for (int i = 0; i < m_list.count() ; ++i) {
		if (m_list[i]) {
			m_list[i]->setCacheEnabled(m_cacheEnabled);
		}
	}
}
int TagList::add(Tag tag, const QString& value)
{
	// To add a value to a list using Tag, the list needs to exist.
	// Otherwise the value must be added using a tag name
	if (tag >= TagLastTag) return -1;
	return m_list[BasicTagMap[tag]]->add(value);
}
int TagList::add(const QString& tagName, const QString& value)
{
	if (!m_tagNameToInt.contains(tagName)) {
		// Create a new StringTagValues item
		m_tagNameToInt.insert(tagName, m_list.count());
		m_list.append(static_cast<TagValues*>(new StringTagValues));
		m_list[BasicTagMap[(Tag)m_tagNameToInt[tagName]]]->setCacheEnabled(m_cacheEnabled);
	}
	return m_list[BasicTagMap[(Tag)m_tagNameToInt[tagName]]]->add(value);
}
int TagList::add(Tag tag, int value)
{
	if (tag >= TagLastTag) return -1;
	return m_list[BasicTagMap[tag]]->add(value);
}
int TagList::add(const QString& tagName, int value)
{
	if (!m_tagNameToInt.contains(tagName)) {
		m_tagNameToInt.insert(tagName, m_tagNameToInt.count());
		m_list.append(new TagValues);
		m_list[BasicTagMap[(Tag)m_tagNameToInt[tagName]]]->setCacheEnabled(m_cacheEnabled);
	}
	return m_list[BasicTagMap[(Tag)m_tagNameToInt[tagName]]]->add(value);
}
QString TagList::tagValue(const QString& tagName, int index)
{
	if (!m_tagNameToInt.contains(tagName)) {
		// If we don't know the tag name, return a empty string.
		return QString();
	}
	return m_list[BasicTagMap[(Tag)m_tagNameToInt[tagName]]]->value(index);
}
QString TagList::tagValue(Tag tag, int index)
{
	// Only predefined tags can be returned using Tag
	// Custom tags must be retrieved using the tag name
	if (tag >= TagLastTag) return QString();
	return m_list[BasicTagMap[tag]]->value(index);
}
TagValues* TagList::tagValueList(int index)
{
	return m_list[BasicTagMap[(Tag)index]];
}

TagValues* TagList::operator[](int index)
{
	return m_list[BasicTagMap[(Tag)index]];
}

TagValues* TagList::operator[](int index) const
{
	return m_list[BasicTagMap[(Tag)index]];
}

int TagList::tagFromString(const QString& tagName)
{
	return (Tag)m_tagNameToInt.value(tagName);
}

QString TagList::stringFromTag(Tag tag)
{
	return m_tagNameToInt.key(tag);
}
int TagList::count()
{
	return m_list.count();
}
void TagList::clear()
{
	for (int i = 0; i < m_list.size(); ++i) {
		if (m_list.at(i)) {
			delete(m_list.at(i));
			m_list[i] = NULL;
		}
	}
	for (int i = TagLastTag; i < m_list.size(); ++i) m_list.removeLast();
}
void TagList::write(QDataStream& out)
{
	out << count();
	for (int i = 0; i < m_list.size(); ++i) {
		m_list[i]->write(out);
	}

}
void TagList::read(QDataStream& in)
{
	int listCount;
	clear();
	in >> listCount;
	for (int i = 0; i < listCount ; ++i) {
		m_list.append(NULL);
		m_list[i] = static_cast<TagValues*>(new StringTagValues);
		m_list[i]->read(in);
	}
}

void TagList::addTagValues(const Tag& tag, TagValues* tagvalues)
{
	if (m_list[tag]) // tagvaluelist already there
		{return;}
	m_list[tag] = tagvalues;
}
