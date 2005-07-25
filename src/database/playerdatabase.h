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
PlayerDatabase is used to acces player data.
*/

#ifndef __PLAYERDATABASE_H__
#define __PLAYERDATABASE_H__

#include <qstring.h>
#include <qmap.h>
#include <qdatetime.h>
#include <qdatastream.h>
#include <qfile.h>
#include <playerdata.h>

class PlayerDatabase{

public:
/**
create a new player database
*/
bool create(const QString& fname);
/**
open an existing player database
*/
bool open(const QString& fname);
/**
remove a player database
*/
bool removeDatabase(const QString& fname);
/**
commit pending updates
*/
void commit();
/**
close the player database
*/
void close();
/**
reclaim unused space in the data file
*/
void compact();
/**
number of players in the database
*/
uint count() const;
/**
add a new player
*/
bool add(const QString& pname);
/**
remove player from database
*/
bool remove(const QString& pname);
/**
returns true iff the player has en entry
in the database
*/
bool exists(const QString& pname);
/**
returns date of birth for a player
format: yyyy.mm.dd
- mm and/or dd may be = ??
TODO: change return type to PartialDate (class to be written)
*/
QString dateOfBirth(const QString& pname);
/**
updates date of birth for a player
*/
void setDateOfBirth(const QString& d, const QString& pname);
/**
returns date of death for a player
format: yyyy.mm.dd
- mm and/or dd may be = ??
*/
QString dateOfDeath(const QString& pname);
/**
updates date of death for a player
*/
void setDateOfDeath(const QString& d, const QString& pname);
/**
returns country for a player
*/
QString country(const QString& pname);
/**
updates country for a player
Standard is 3 character country code 
(RUS, DEN, SWE, etc.)
Multiple countries can be provided by separating them by '/',
fe. RUS/GER (earliest country first)
*/
void setCountry(const QString& s, const QString& pname);
/**
returns title for a player
*/
QString title(const QString& pname);
/**
updates title for a player
 gm: Grandmaster
 im: International Master
 fm: FIDE Master
 wgm: Woman Grandmaster
 wim: Woman International Master
 wfm: Woman FIDE Master
 cgm: Correspondence GM
 cim: Correspondence IM
 hgm: Honorary Grandmaster
 comp: Computer
 -: None/unknown
Multiple titles can be provided by separating them by '+',
fe. gm+cgm
*/
void setTitle(const QString& s, const QString& pname);
/**
players elo at the given date. Official elo
at the date if available, otherwise peak rating
or estimated rating.
*/
int elo(const QString& pname, const QDate& date);
/**
updates official elo rating for a player, for a given year
*/
void setOfficialElo(const Q_INT32 year, const QMemArray<int> ar, const QString& pname);
/**
updates peak elo rating for a player
*/
void setPeakElo(const int i, const QString& pname);
/**
updates estimated elo rating for a player
*/
void setEstimatedElo(const int i, const QString& pname);
/**
returns photo for a player
*/
QImage photo(const QString& pname);
/**
updates photo for a player
*/
void setPhoto(const QImage& img, const QString& pname);
/**
returns biography for a player
*/
QString biography(const QString& pname);
/**
updates biography for a player
*/
void setBiography(const QString& s, const QString& pname);
void appendToBiography(const QString& s, const QString& pname);
/**
returns a list of all player names in database
*/
QStringList playerNames();

/**
returns a list of all player names in database,
matching the prefix
*/
QStringList findPlayers(const QString& prefix, int maxCount = 10000000);

private:
QMap<QString,Q_INT32> mapping; // pointers into data
QDataStream mapds; // contains mapping
QFile mapfile; 
Q_INT32 nplayers; // number of players in db
QIODevice::Offset nplayers_offset; // position in mapfile
QDataStream datads; // contains data
QFile datafile; 
QIODevice::Offset dataFileCurrentPosition; // current position

QMap<QString,PlayerData> pendingUpdates; // not committed yet
QMap<QString, PlayerData>::Iterator prepareUpdate(const QString & pname);

QMap<QString,PlayerData> cache; // for reading
QMap<QString, PlayerData>::Iterator refreshCache(const QString & pname);

PlayerData readPlayerData(const QString & pname);

};

#endif
