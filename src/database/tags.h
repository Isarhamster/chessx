/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 28/05/2005
    copyright            : (C) 2005 Ejner Borgbjerg 
                           <ejner@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
One instance holds all the tag values used in a game database.
Documentation for managing QDataset format version, see
http://doc.trolltech.com/4.0/qdatastream.html
Some ideas for the file format are borrowed from Scid (see namebase.cpp
and tkscid.cpp).
*/
#ifndef __TAGS_H__
#define __TAGS_H__

#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QDataStream>
#include <QFile>
#include <QVector>
#include <QMap>
#include <QPair>

/*TODO
-compact(): 
 Needs access to index file for the games, but for now just create index mapping.
 May also clean up unused custom tag id's.
-maybe some functions can be optimized with const_iterators or just loops
-file may be even smaller by having id/frequency use smaller types when possible.
-QByteArray (in Qt4) can perhaps be used for read/write with compression ?
*/

class Tags{

public:
/**
Construct with given filename.
*/
Tags(const QString& fname);
Tags();
/**
Write the tags file.
*/
void writeFile();
/**
Read in tags file.
@return true iff succesful.
*/
bool readFile();
/**
Remove the tags file.
@return true iff succesful.
*/
bool removeFile();
/**
Remove unused values, saving space.
Not implemented yet, it involves also writing to index file.
*/
void compact();
/**
The different kinds of tags cf.
http://www.tim-mann.org/Standard or
http://www.very-best.de/pgn-spec.htm#8.1
*/
enum KnownTagTypes {
// mandatory tags
Event, Site, Date, Round, PlayerName,Result,
// the rest is optional tags
// player
PlayerTitle, PlayerElo, PlayerUSCF, PlayerNA, PlayerType,
//event
EventDate, EventSponsor, Section, Stage, Board,
//opening
Opening, Variation, Subvariation, ECO, NIC, 
//time/date
Time, UTCTime, UTCDate, 
//time control
TimeControl, 
//starting position
SetUp, FEN, 
//game conclusion
Termination, 
//misc
Annotator, Mode, PlyCount, 
Source
};
/**
Return the value for the given valueId and tagId.
Returns value even if frequency is 0.
*/
QString value(const uint tagId, const int valueId) const;
/**
Return the valueId for the given value and tagId.
*/
int valueId(const uint tagId, const QString& value) const;
/**
Return the frequency for the given valueId and tagId.
*/
uint valueFrequency(const uint tagId, const int valueId) const;
/**
Return a bitvector indicating which valueId's
for the given tagId has the given string as a prefix.
*/
QVector<bool> find(const uint tagId, const QString& pattern);
/**
Return a bitvector indicating which valueId's
for the given tagId matches the given regex.
*/
QVector<bool> find(const uint tagId, const QRegExp& pattern);
/**
Add the value to the value collection for the given tagId.
@return new index if it is not there.
@return existing index if tag value is already there.
In both cases, the frequency for the value is incremented.
*/
int add(const uint tagId, const QString& value);
/**
Decrement the frequency of the given value in the value collection for the given tagId.
Actual removal of a value can only happen in compact(), and then only if the values frequency is 0.
*/
void remove(const uint tagId, const int valueId);
/**
@return Number of different values of the given tagId.
Values with 0 frequency are counted as well.
*/
int count(const uint tagId) const;
/**
@return true iff tag is known.
*/
bool isKnown(const uint tagId) const;
/**
@return true iff tag is known and mandatory.
*/
bool isKnownMandatory(const uint tagId) const;
/**
@return true iff tag is known and optional.
*/
bool isKnownOptional(const uint tagId) const;
/**
@return true iff tag is custom (user defined).
*/
bool isCustom(const uint tagId) const;
/**
Add a custom tag name, and @return its index in tag name
collection. If the tag name exists, -1 is returned.
*/
int defineTag(const QString& tagName);
/**
Remove a custom tag name.
All values for the tag are also deleted. 
Known tag names can't be removed.
@return true iff removal succeeded.
*/
bool unDefineTag(const QString& tagName);
/**
The different kinds of tag names.
*/
enum TagNameType{All, Known, KnownOptional, KnownMandatory, Custom};
/**
@return list of tag names.
Default is to return all (both known and custom) tag names.
*/
QStringList tagNames(const TagNameType type = All);
/**
@return tagId for the given tag name.
@return -1 if tagId is undefined.
Works both with known and custom tags.
*/
int tagId(const QString& tagName) const;
/**
@return tag name for the given tagId.
Works both with known and custom tags.
*/
QString tagName(const uint tagId) const;


private:
QString m_fname;//filename
QDataStream m_tags_ds;//datastream
QFile m_tags_file; //file

//tag value collections (known as well as custom)
QMap <uint, QPair <QVector<QString>, QMap <QString,QPair<quint32,quint32> > > > m_allTags;

//for maintaining custom tag name definitions
QVector<QString> m_customTags_v;
QMap<QString,QPair<uint,bool> > m_customTags_m;//<uint,bool> is tagId and used flag

void clear();
void closeFile();

QVector<bool> find(const QString& pattern, QVector<QString>& vector, QMap<QString,QPair<quint32,quint32> >& map);
QVector<bool> find(const QRegExp& pattern, QVector<QString>& vector, QMap<QString,QPair<quint32,quint32> >& map);
static int knownTagIndex(const QString& tagName);
static uint maxKnownTagIndex();
static uint minKnownTagIndex();

};

#endif
