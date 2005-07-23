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
*/
#include "playerdatabase.h"

static QString m_magic = "ChessX name db"; // 'magic' string
static QString m_version = "1.0"; // file format version
static QString m_mapfile_suffix = ".cpm";
static QString m_datafile_suffix = ".cpd";

bool PlayerDatabase::create(const QString & fname) {
  mapfile.setName(fname+m_mapfile_suffix);
  datafile.setName(fname+m_datafile_suffix );
  if (mapfile.exists()||datafile.exists())
     return false;

  mapfile.open( IO_ReadWrite );
  mapds.setDevice(&mapfile);
  mapds << m_magic;
  mapds << m_version;
  nplayers = 0;
  nplayers_offset = mapfile.at();
  mapds << nplayers;

  datafile.open( IO_ReadWrite );
  datads.setDevice(&datafile);
  datads << m_magic;
  datads << m_version;

  mapfile.flush();
  datafile.flush();

  dataFileCurrentPosition = datafile.at();

  return true;
}

bool PlayerDatabase::open(const QString & fname) {
  mapfile.setName(fname+m_mapfile_suffix);
  mapfile.open( IO_ReadWrite );
  mapds.setDevice(&mapfile);
  QString magic;
  mapds >> magic;
  QString version;
  mapds >> version;

  if (version==m_version && magic==m_magic){
     nplayers_offset = mapfile.at();
     mapds >> nplayers;
     if (nplayers>0){
        mapds >> mapping;
     }
     datafile.setName(fname+m_datafile_suffix);
     datafile.open( IO_ReadWrite );
     datads.setDevice(&datafile);
     datads >> magic;
     datads >> version;
     if (version==m_version && magic==m_magic){
       dataFileCurrentPosition = datafile.at();
       return true;
     }
     else{
       close();
       return false;
     }
  }

  else{
     close();
     return false;
  }
}

bool PlayerDatabase::remove(const QString & fname) {
  mapfile.setName(fname+m_mapfile_suffix);
  datafile.setName(fname+m_datafile_suffix);
  return mapfile.remove() && datafile.remove();
}

void PlayerDatabase::close() {
  commit();
  mapds.unsetDevice();
  mapfile.flush();
  mapfile.close();
  datads.unsetDevice();
  datafile.flush();
  datafile.close();
}

void PlayerDatabase::commit() {

  mapfile.at(nplayers_offset);
  mapds << nplayers;

  // write files from pendingUpdates
  dataFileCurrentPosition = datafile.size();
  datafile.at(dataFileCurrentPosition);

  QMap<QString, PlayerData>::Iterator it;  
  for ( it = pendingUpdates.begin(); it != pendingUpdates.end(); ++it ) {
     mapping.insert(it.key(),datafile.at());
     datads << it.data().dateOfBirth();
     datads << it.data().dateOfDeath();
     datads << it.data().country();
     datads << it.data().title();
     datads << it.data().eloList();
     datads << it.data().estimatedElo();
     datads << it.data().peakElo();
     datads << it.data().photo();
     datads << it.data().biography();
  }
  mapds << mapping;
  mapfile.flush();
  datafile.flush();
  pendingUpdates.clear();
}

void PlayerDatabase::compact() {
}

uint PlayerDatabase::numberOfPlayers() const {
  return nplayers;
}

bool PlayerDatabase::add(const QString & pname) {
  if (mapping.contains(pname)||pendingUpdates.contains(pname)){
     return false;
  }
  PlayerData p;
  pendingUpdates.insert(pname,p);
  nplayers++;
  return true;
}

PlayerData PlayerDatabase::readPlayerData(const QString & pname){
    PlayerData pd;
    QMap<QString, Q_INT32>::Iterator it;
    it = mapping.find(pname);
    if (it==mapping.end()){
//look in pending updates
      QMap<QString,PlayerData>::Iterator it2; 
      it2 = pendingUpdates.find(pname);
      if (it2!=pendingUpdates.end()){
        return it2.data();
      }
      else{//give up
        return pd;
      }
    }


    Q_INT32 pos = it.data();
    datafile.at(pos);//pointing to the player data

    QString birthDate;
    QString deathDate;
    QString country;
    QString title;

    datads >> birthDate;
    datads >> deathDate;
    datads >> country;
    datads >> title;

    pd.setDateOfBirth(birthDate);
    pd.setDateOfDeath(deathDate);
    pd.setCountry(country);
    pd.setTitle(title);
    QValueList<Q_INT32> eloList;
    datads >> eloList;

    for(uint i=0; i<eloList.size();){
      Q_INT32 year = eloList[i++];
      Q_INT32 n_ratings = eloList[i++];
      QMemArray<int> ar(n_ratings);
      for(int j=0; j<n_ratings; j++){
        ar[j] = eloList[i++];
      }
      pd.setOfficialElo(year,ar);
    }

    Q_INT32 estimatedElo;
    Q_INT32 peakElo;
    QImage photo;
    QString biography;

    datads >> estimatedElo;
    datads >> peakElo;
    datads >> photo;
    datads >> biography;

    pd.setEstimatedElo(estimatedElo);
    pd.setPeakElo(peakElo);
    pd.setPhoto(photo);
    pd.setBiography(biography);

    return pd;
}

QMap<QString, PlayerData>::Iterator PlayerDatabase::refreshCache(const QString & pname){
  QMap<QString, PlayerData>::Iterator iter = cache.find(pname);
  if (iter!=cache.end()){
    return iter;
  }
  else{
    if (!cache.empty()){//very simple 1-element cache...
      cache.clear();
    }
    return cache.insert(pname,readPlayerData(pname));
  }
}

QMap<QString, PlayerData>::Iterator PlayerDatabase::prepareUpdate(const QString & pname){
  QMap<QString, PlayerData>::Iterator iter = pendingUpdates.find(pname);
  if (iter!=pendingUpdates.end()){
    return iter;
  }
  else{
    return pendingUpdates.insert(pname,readPlayerData(pname));
  }
}

bool PlayerDatabase::exists(const QString & pname){
    QMap<QString, Q_INT32>::Iterator it;
    it = mapping.find(pname);
    if (it==mapping.end()){
//not in committed db - look in pending updates
      QMap<QString,PlayerData>::Iterator it2; 
      it2 = pendingUpdates.find(pname);
      if (it2==pendingUpdates.end()){
        return false;
      }
    }
    return true;
}

QString PlayerDatabase::dateOfBirth(const QString & pname){
  return refreshCache(pname).data().dateOfBirth();
}
void PlayerDatabase::setDateOfBirth(const QString & pname, const QString & d){
  prepareUpdate(pname).data().setDateOfBirth(d);
}

QString PlayerDatabase::dateOfDeath(const QString & pname){
  return refreshCache(pname).data().dateOfDeath();
}
void PlayerDatabase::setDateOfDeath(const QString & pname, const QString & d){
  prepareUpdate(pname).data().setDateOfDeath(d);
}

QString PlayerDatabase::country(const QString & pname){
  return refreshCache(pname).data().country();
}
void PlayerDatabase::setCountry(const QString & pname, const QString & s){
  prepareUpdate(pname).data().setCountry(s);
}

QString PlayerDatabase::title(const QString & pname){
  return refreshCache(pname).data().title();
}
void PlayerDatabase::setTitle(const QString & pname, const QString & s){
  prepareUpdate(pname).data().setTitle(s);
}

int PlayerDatabase::elo(const QString & pname, const QDate & date){
  return refreshCache(pname).data().elo(date);
}
void PlayerDatabase::setOfficialElo(const QString & pname, const Q_INT32 year,  const QMemArray<int> ar){
  prepareUpdate(pname).data().setOfficialElo(year,ar);
}

void PlayerDatabase::setPeakElo(const QString & pname, const int i){
  prepareUpdate(pname).data().setPeakElo(i);
}

void PlayerDatabase::setEstimatedElo(const QString & pname, const int i){
  prepareUpdate(pname).data().setEstimatedElo(i);
}

QImage PlayerDatabase::photo(const QString & pname){
  return refreshCache(pname).data().photo();
}
void PlayerDatabase::setPhoto(const QString & pname, const QImage & img){
  prepareUpdate(pname).data().setPhoto(img);
}

QString PlayerDatabase::biography(const QString & pname){
  return refreshCache(pname).data().biography();
}
void PlayerDatabase::setBiography(const QString & pname, const QString & s){
  prepareUpdate(pname).data().setBiography(s);
}
void PlayerDatabase::addToBiography(const QString & pname, const QString & s){
  prepareUpdate(pname).data().addToBiography(s);
}

QStringList PlayerDatabase::playerNames(){
  QStringList result;
  QMap<QString,Q_INT32>::Iterator it;
  for ( it = mapping.begin(); it != mapping.end(); ++it ) {
     result.push_back(it.key());
  }
  return result;
}
