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
*/
#ifndef __TAGS_H__
#define __TAGS_H__

#include <qstring.h>
#include <qregexp.h>
#include <qdatastream.h>
#include <qfile.h>
#include <qt34/qvaluevector.h>
#include <qmap.h>
#include <qpair.h>

/*TODO
-custom tags
-file format optimizing. see Scid's namebase.cpp
 (raw binary format, string frontcoding...)
-compact(): needs access to index file for the games
 cf. Scid's sc_compact_names in tkscid.cpp
*/

class Tags{

public:
/**
create a new tags file
*/
bool create(const QString& fname);
/**
open an existing tags file
*/
bool open(const QString& fname);
/**
close the active tags file
*/
void close();
/**
remove a tags file
*/
bool removeTagsFile(const QString& fname);
/**
the different kinds of tags cf.
http://www.tim-mann.org/Standard
or
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
return the value for the given index and tag type
*/
QString value(uint tagId, int index);
/**
return the index for the given name and tag type
*/
int index(uint tagId, const QString& name);
/**
return a bitvector with information indicating which indices 
for the given tag type has the given string as a prefix.
*/
QValueVector<bool> find(uint tagId, const QString& pattern);
/**
return a bitvector with information indicating which indices 
for the given tag type matches the given regex.
*/
QValueVector<bool> find(uint tagId, const QRegExp& pattern);
/**
add the name to the tag value collection for the given tag type.
return new index if it is not there;
return existing index if tag value is already there
*/
int add(uint tagId, const QString& name); 
/**
remove the name from the tag value collection for the given tag type.
*/
void remove(uint tagId, int index);
/**
@return number of tag values of the given tag type.
unused values are counted as well.
*/
int count(uint tagId);
/**
explicitly flush the tag file.
*/
void flushTagsFile();
/**
physically erase unused tag values
*/
void compact();
/**
@return the size of the tag file
*/
int tagFileSize();
/**
@return true iff tag is known
*/
bool isKnown(uint tagId);
/**
@return true iff tag is known and mandatory
*/
bool isMandatory(uint tagId);
/**
@return true iff tag is known and optional
*/
bool isOptional(uint tagId);
/**
@return true iff tag is custom (user defined)
*/
bool isCustom(uint tagId);
/**
add a custom tag name, and @return its index in tag name
collection. If the tag name exists, its index is returned.
tagName may not be one of the known tag names. If it is, -1 will
be returned instead.
*/
int addTagName(QString& tagName);
/**
remove a custom tag name by its index in the tag names collection
All values for the tag are also deleted. 
Should only be called if the tag name is not used.
It's not allowed to remove known tag names.
@return true iff removal succeeded.
*/
bool removeTagName(uint tagId);
/**
The different kinds of tag names.
*/
enum TagNameType{All, Known, KnownMandatory, KnownOptional, Custom};
/**
@return tag names of the given type.
Default is to return all types of tag names.
*/
QStringList tagNames(TagNameType type = All);
/**
@return index tag name collection
@return -1 if it is not there
*/
int tagIndex(QString& tagName);
/**
@return name of tag with the given index
*/
QString tagName(uint tagId);


private:
QDataStream m_tags_ds; 
QFile m_tags_file; 

//TODO 
//tag names collections
//QValueVector<QString> m_tagname_vector;
//QMap<QString,int> m_tagname_map;

QMap <uint, QPair <QValueVector<QString>, QMap <QString,int> > > m_allTags;

QValueVector<bool> find(const QString& pattern, QValueVector<QString>& vector, QMap<QString,int>& map);
QValueVector<bool> find(const QRegExp& pattern, QValueVector<QString>& vector, QMap<QString,int>& map);

};

#endif
