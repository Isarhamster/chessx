/***************************************************************************
 *   (C) 2006 Ejner Borgbjerg <ejner@users.sourceforge.net>                *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef PLAYERDATA_H_INCLUDED
#define PLAYERDATA_H_INCLUDED

#include <QString>
#include <QImage>
#include <QMap>
#include <QList>
#include "partialdate.h"

/** @ingroup Database
The PlayerData class holds information about single player.
*/
class PlayerData
{
public:
    PlayerData();
    /**
    Players date of birth, if it is known.
    */
    PartialDate dateOfBirth() const;
    void setDateOfBirth(const PartialDate &d);
    PartialDate dateOfDeath() const;
    void setDateOfDeath(const PartialDate &d);
    QString country() const;
    void setCountry(const QString &s);
    QString title() const;
    void setTitle(const QString &s);
    int firstEloListIndex();
    void setFirstEloListIndex(int ix);
    int lastEloListIndex();
    void setLastEloListIndex(int ix);
    /**
    players elo from the given elo list.
    */
    int elo(int eloList) const;
    void setElo(const int eloList, int elo);
    /**
    the estimated elo for the player at the elo list
    with the given index: if the player is not in the list,
    the closest previous elo list is used if available;
    else the overall estimate is used.
    Non-const due to caching.
    */
    int estimatedElo(int eloListIndex);
    /**
    Like estimatedElo(int), but no caching is used.
    */
    int estimatedEloNoCache(int eloListIndex) const;
    int peakElo() const;
    void setPeakElo(const int elo);
    /**
    the overall estimated elo for the player
    */
    int estimatedElo() const;
    void setEstimatedElo(int elo);
    QImage photo() const;
    void setPhoto(const QImage img);
    QString biography() const;
    void setBiography(const QString &str);
    void appendToBiography(const QString &str);
    /**
    returns the elo list data as a QList<qint32>.
    Useful for writing the data out to a QDataStream.
    */
    QList<qint32> eloListData() const;
    /**
    sets the elo list data from a QList<qint32>.
    Useful for reading in the data from a QDataStream.
    */
    void eloFromListData(const QList<qint32> &eloListData);

private:
    PartialDate m_dateOfBirth;
    PartialDate m_dateOfDeath;
    QString m_country;
    QString m_title;
    QMap<int, int> m_elo; // key is elo list index, value is rating for player from the list
    QMap<int, int> m_estimatedEloCache; // key is elo list index, value is latest previous rating for player
    int m_estimatedElo; // for historic players
    int m_peakElo; // highest elo
    int m_firstEloListIndex; // index of first elo list where player is represented
    int m_lastEloListIndex; // index of first elo list where player is represented
    QImage m_photo;
    QString m_biography; // html string

};

#endif
