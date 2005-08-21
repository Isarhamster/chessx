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
One instance holds the tag values used in a game database.
Documentation for managing QDataset version, see
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
the different kinds of tags
*/
enum TagType {Name, Event, Site, Round};
/**
return the name for the given index and tag type
*/
QString name(TagType t, int index);
/**
return the index for the given name and tag type
*/
int index(TagType t, const QString& name);
/**
return a vector with information about which indices that matches the given pattern
*/
QValueVector<bool> find(TagType t, const QString& pattern);
/**
return a NameList with all tag values that matches the given regex and tag type
*/
QValueVector<bool> find(TagType t, const QRegExp& pattern);
/**
add the name to the tag value collection for the given tag type.
return new index if it is not there;
return existing index if tag value is already there
*/
int add(TagType t, const QString& name); 
         /* add new name if it is not there, return existing index if it is
            there - should be quite fast */
/**
remove the name from the tag value collection for the given tag type.
*/
void remove(TagType t, int index);
/**
number of tag values of the given tag type.
unused values are counted as well.
*/
int count(TagType t);
/**
explicitly flush the tag file.
*/
void flushTagsFile();
/**
physically erase unused tag values
*/
void compact();


private:
QDataStream m_tags_ds; 
QFile m_tags_file; 

QValueVector<QString> m_name_vector;
QMap<QString,int> m_name_map;

QValueVector<QString> m_event_vector;
QMap<QString,int> m_event_map;

QValueVector<QString> m_site_vector;
QMap<QString,int> m_site_map;

QValueVector<QString> m_round_vector;
QMap<QString,int> m_round_map;

int addIndex(TagType& tp, QValueVector<QString>& vector, QMap<QString,int>& map, const QString& name);

QValueVector<bool> find(TagType t, const QString& pattern, QValueVector<QString>& vector, QMap<QString,int>& map);

QValueVector<bool> find(TagType t, const QRegExp& pattern, QValueVector<QString>& vector, QMap<QString,int>& map);

void remove(TagType t, int index, QValueVector<QString>& vector, QMap<QString,int>& map, const QString& prefix);

};

#endif
