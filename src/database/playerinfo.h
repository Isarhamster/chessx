/***************************************************************************
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include "common.h"
#include "partialdate.h"

#include <QString>
#include <QList>
#include <QPair>

class Database;

class PlayerInfo {
public:
	/** Standard constructor. Does nothing. */
	PlayerInfo();
	/** Set database and player. Automatically updates information. */
	PlayerInfo(Database* db, const QString& player);
	/** Standard destructor. */
	~PlayerInfo();
	/** Changes database. Does not automatically update statistics. */
	void setDatabase(Database* db);
	/** Changes player. Does not automatically update statistics. */
	void setName(const QString& player);
	/** @return current player name. */
	QString name() const;
	/** Recalculates all statistics. */
	void update();
	/** @return string with formatted score statistics. */
	QString formattedScore() const;
	/** @return string with formatted date range. */
	QString formattedRange() const;
	/** @return string with formatted rating. */
	QString formattedRating() const;
    /** @return string with formatted game count. */
    QString formattedGameCount() const;

    /** @return string with unformatted score statistics. */
    QString unformattedScore() const;
    /** @return string with unformatted score statistics. */
    QString unformattedScoreTotal() const;
    /** @return string with unformatted score statistics. */
    QString unformattedScoreWhite() const;
    /** @return string with unformatted score statistics. */
    QString unformattedScoreBlack() const;
    /** @return string with unformatted date range. */
    QString unformattedRange() const;
    /** @return string with unformatted rating. */
    QString unformattedRating() const;
    /** @return string with unformatted game count. */
    QString unformattedGameCount() const;

private:
	/** Clears all statistics. */
	void reset();
	/** Add statistics for game @p index when player has given color. */
	void addGameStats(int index, Color color);
    /** @return string with unformatted score statistics. */
    QString unformattedScore(const int results[4], int count) const;
    /** Format score statistics for single color. */
	QString formattedScore(const int results[4], int count) const;
	typedef QList<QPair<QString, unsigned> > EcoFrequency;
	QString m_name;
	Database* m_database;
	int m_result[2][4];
	int m_count[2];
	int m_rating[2];
	EcoFrequency m_eco[2];
	PartialDate m_date[2];
};

#endif
