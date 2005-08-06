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

#ifndef __PLAYERDATA_H__
#define __PLAYERDATA_H__

#include <qstring.h>
#include <qimage.h>
#include <qdatetime.h>
#include <qmap.h>
#include <qt34/qvaluelist.h>
#include <qt34/qmemarray.h>
#include <partialdate.h>

class PlayerData
{
public:
  PlayerData();
/**
Players date of birth, if it is known.
*/
  PartialDate dateOfBirth() const;
  void setDateOfBirth(const PartialDate d);
  PartialDate dateOfDeath() const;
  void setDateOfDeath(const PartialDate d);
  QString country() const;
  void setCountry(const QString s);
  QString title() const;
  void setTitle(const QString s);
/**
players elo from the given elo list.
*/
  int elo(const int eloList) const;
  void setElo(const int eloList, const int elo);
/**
the estimated elo for the player at the elo list
with the given index: if the player is not in the list,
the closest previous elo list is used if available;
else the overall estimate is used.
Non-const due to caching.
*/
  int estimatedElo(const int eloListIndex);
/**
Like estimatedElo(int), but no caching is used.
*/
  int estimatedEloNoCache(const int eloListIndex) const;
  int peakElo() const;
  void setPeakElo(const int elo);
/**
the overall estimated elo for the player
*/
  int estimatedElo() const;
  void setEstimatedElo(const int elo);
  QImage photo() const;
  void setPhoto(const QImage img);
  QString biography() const;
  void setBiography(const QString str);
  void appendToBiography(const QString str);
/**
returns the elo list data as a QValueList<Q_INT32>.
Useful for writing the data out to a QDataStream.
*/
  QValueList<Q_INT32> eloListData() const;
/**
sets the elo list data from a QValueList<Q_INT32>.
Useful for reading in the data from a QDataStream.
*/
  void eloFromListData(const QValueList<Q_INT32> eloListData);

private:
  PartialDate m_dateOfBirth;
  PartialDate m_dateOfDeath;
  QString m_country;
  QString m_title;
  QMap<int,int> m_elo; // key is elo list index, value is rating for player from the list
  QMap<int,int> m_estimatedEloCache; // key is elo list index, value is latest previous rating for player
  int m_estimatedElo; // for historic players
  int m_peakElo; // highest elo
  QImage m_photo;
  QString m_biography; // html string

};

#endif
