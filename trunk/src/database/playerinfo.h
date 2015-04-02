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
struct EcoFrequencyInfo
{
    unsigned int count;
    int result[4];
    EcoFrequencyInfo()
    {
        count = 0;
        memset(result, 0, sizeof(result));
    }
};

typedef QPair<QString, EcoFrequencyInfo> EcoFrequencyItem;
typedef QList<EcoFrequencyItem> EcoFrequency;
typedef QPair<QString, int> OpeningCountItem;
typedef QHash<QString, int> OpeningCountMap;
typedef QList<OpeningCountItem> OpeningCountList;

class PlayerInfo
{
public:
    /** Standard constructor. Does nothing. */
    PlayerInfo();
    /** Set database and player. Automatically updates information. */
    PlayerInfo(Database* db, const QString& player);
    /** Standard destructor. */
    ~PlayerInfo();
    /** Changes database. Does not automatically update statistics. */
    void setDatabase(Database* db);
    /** Changes player. Update statistics automatically. */
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
    /** @return string with list of openings played */
    QString listOfOpenings() const;

private:
    /** Clears all statistics. */
    void reset();
    /** Add statistics for game @p index when player has given color. */
    void addGameStats(int index, Color color);
    /** Format score statistics for single color. */
    QString formattedScore(const int results[4], int count) const;
    int toResult(const QString& res) const;

    QString m_name;
    Database* m_database;
    int m_result[2][4];
    int m_count[2];
    int m_rating[2];
    EcoFrequency m_eco[2];
    PartialDate m_date[2];
    OpeningCountList m_opening[2];

};

#endif
