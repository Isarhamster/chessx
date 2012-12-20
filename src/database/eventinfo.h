/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef EVENTINFO_H
#define EVENTINFO_H

#include "common.h"
#include "partialdate.h"

#include <QString>
#include <QList>
#include <QPair>

class Database;

class EventInfo
{
public:
    EventInfo();
public:
    /** Set database and player. Automatically updates information. */
    EventInfo(Database* db, const QString& player);
    /** Standard destructor. */
    ~EventInfo();
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

private:
    /** Clears all statistics. */
    void reset();
    /** Add statistics for game @p */
    void addGameStats(int index);

    QString m_name;
    Database* m_database;

    /** Format score statistics for single color. */
    QString formattedScore(const int results[4], int count) const;
    int toResult(const QString& res) const;

    typedef QList<QPair<QString, unsigned> > EcoFrequency;
    int m_result[4];
    int m_count;
    int m_rating[2];
    EcoFrequency m_eco;
    PartialDate m_date[2];

};

#endif // EVENTINFO_H
