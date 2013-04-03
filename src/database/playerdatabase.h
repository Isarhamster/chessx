/***************************************************************************
 *   (C) 2006 Ejner Borgbjerg <ejner@users.sourceforge.net>                * 
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

/** @ingroup Database
The PlayerDatabase class is used to acces player data.

@todo
- optimize storage - QString is said to serialize as UTF-16
- maybe consider changing to a portable pure C++ format instead?
*/

#ifndef __PLAYERDATABASE_H__
#define __PLAYERDATABASE_H__

#include <QString>
#include <QMap>
#include <QDataStream>
#include <QFile>
#include <QStringList>
#include "playerdata.h"

class PlayerDatabase
{

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
	rollback - cancel pending updates
	*/
	void rollback();
	/**
	close the player database
	*/
	void close();
	/**
	number of players in the database
	*/
	uint count() const;
	/**
	add a new player
	*/
	bool add(const QString& playername);
	/**
	name of the current player object
	*/
	QString current() const;
	/**
	makes the player the current object for later query/update
	function calls.
	*/
	void setCurrent(const QString& playername);
	/**
	returns true iff the player has en entry
	in the database
	*/
	bool exists(const QString& playername) const;
	/**
	returns date of birth for a player
	*/
	PartialDate dateOfBirth() const;
	/**
	updates date of birth for a player
	*/
	void setDateOfBirth(const PartialDate& date);
	/**
	returns date of death for a player
	*/
	PartialDate dateOfDeath() const;
	/**
	updates date of death for a player
	*/
	void setDateOfDeath(const PartialDate& date);
	/**
	returns country for a player
	*/
	QString country() const;
	/**
	updates country for a player
	Standard is 3 character country code
	(RUS, DEN, SWE, etc.)
	Multiple countries can be provided by separating them by '/',
	fe. RUS/GER (earliest country first)
	*/
	void setCountry(const QString& country);
	/**
	returns title for a player
	*/
	QString title() const;
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
	void setTitle(const QString& title);
	/**
	Index of the first elo list where player is represented
	*/
	int firstEloListIndex();
	/**
	Index of the last elo list where player is represented
	*/
	int lastEloListIndex();
	/**
	players elo rating from the elo list with the given index.
	In period from 1971 to 2000, there is 2 lists pr. year;
	The first list from 1971 has index = 1, and the last list of
	2000 has index = 60.
	Since 2001, there is 4 lists pr. year.
	If player is not in the list, 0 is returned.
	*/
	int elo(const int eloListIndex) const;
	/**
	players elo rating at the given date.
	If player is not in the elolist on that date, 0 is returned.
	*/
	int elo(const PartialDate& date) const;
	/**
	Like elo(const QDate&), except when player is not in the actual elo list:
	Return rating from nearest previous list if available, overall estimation
	if no previous data available, 0 if nothing is available.
	Non-const due to caching. The cache can be useful, if calling repeatedly with
	a date inside the same elo list period (cf. the elo(int) function).
	*/
	int estimatedElo(const PartialDate& date);
	/**
	Like estimatedElo(const QDate&), but no caching is done.
	*/
	int estimatedEloNoCache(const PartialDate& date) const;
	/**
	highest overall elo achieved by current player
	*/
	int highestElo() const;
	/**
	overall elo estimation
	*/
	int estimatedElo() const;
	/**
	updates the current players elo for a given list in a given year
	*/
	void setElo(const int year, const int listIndex, const int elo);
	/**
	updates (overall) estimated elo for a player
	*/
	void setEstimatedElo(const int elo);
	/**
	returns true iff a photo of the current player
	is available; the photo can be accessed by photo()
	*/
	bool hasPhoto() const;
	/**
	returns photo for the current player
	if no photo is available, photo().isNull()
	*/
	QImage photo() const;
	/**
	updates photo for a player
	*/
	void setPhoto(const QImage& img);
	/**
	returns true iff a biography for the current player
	is available; the biography can be accessed by biography()
	*/
	bool hasBiography() const;
	/**
	returns biography for a player
	if no biography is available, biography().isNull()
	*/
	QString biography() const;
	/**
	updates biography for a player
	*/
	void setBiography(const QString& s);
	void appendToBiography(const QString& s);
	/**
	returns a list of all player names in database
	*/
	QStringList playerNames();
	/**
	returns a list of player names in database,
	matching the prefix
	By default, max 10000000 names are returned
	By default, search is case sensitive
	*/
	QStringList findPlayers(const QString& prefix, const int maxCount = 10000000, const Qt::CaseSensitivity cs = Qt::CaseSensitive);

	/**
	returns the date for a given elo list index
	*/
	PartialDate eloListToDate(const int index);

private:
	QMap<QString, qint32> m_mapping; // pointers into data
	QDataStream m_mapds; // contains mapping
	QFile m_mapfile;
	qint32 m_nplayers; // number of players in db - not counting pending add's
	qint32 m_npending_adds; // number of pending add's to db
	qint64 m_nplayers_offset; // position of count field in mapfile
	QDataStream m_datads; // contains data
	QFile m_datafile;
	qint64 m_dataFileCurrentPosition; // current position in datafile
	QMap<QString, PlayerData> m_pendingUpdates; // changes, not committed yet
	QString m_currentPlayerName;
	PlayerData m_currentPlayer;
	bool m_dirty;
	PlayerData readPlayerData(const QString & playername);
	int eloList(const PartialDate date) const;
	int eloList(const int year, const int index) const;

};

#endif
