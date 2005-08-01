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

class PlayerData
{
public:
  PlayerData();
/**
Players date of birth, if it is known.
The format is yyyy.mm.dd
For some players only year or year+month is known;
that's why QString is used instead of QDate.
*/
  QString dateOfBirth() const;
  void setDateOfBirth(const QString s);
  QString dateOfDeath() const;
  void setDateOfDeath(const QString s);
  QString country() const;
  void setCountry(const QString s);
  QString title() const;
  void setTitle(const QString s);
/**
players elo at the given date. Official elo
at the date if available, otherwise peak rating
or estimated rating.
*/
  int elo(const QDate dt) const;
  void setOfficialElo(const Q_INT32 year, const QMemArray<int> ar);
  Q_INT32 peakElo() const;
  void setPeakElo(const int i);
  Q_INT32 estimatedElo() const;
  void setEstimatedElo(const int i);
  QImage photo() const;
  void setPhoto(const QImage img);
  QString biography() const;
  void setBiography(const QString s);
  void appendToBiography(const QString s);
/**
returns the official elo data as a QValueList<Q_INT32>, for serialization via
QDataStream
*/
  QValueList<Q_INT32> eloList();

private:
  QString m_dateOfBirth;
  QString m_dateOfDeath;
  QString m_country;
  QString m_title;
  QMap<Q_INT32,QMemArray<int> > m_elo; // key is year, ratings are held in the
                                           // array
  int m_estimatedElo; // for historic players
  int m_peakElo; // highest elo entry
  QImage m_photo;
  QString m_biography; // html string

};

#endif
