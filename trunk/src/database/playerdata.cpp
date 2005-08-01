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
#include <playerdata.h>

PlayerData::PlayerData(){
}

QString PlayerData::dateOfBirth() const{
  return m_dateOfBirth;
}
void PlayerData::setDateOfBirth(const QString d){
  m_dateOfBirth = d;
}
QString PlayerData::dateOfDeath() const{
  return m_dateOfDeath;
}
void PlayerData::setDateOfDeath(const QString d){
  m_dateOfDeath = d;
}

QString PlayerData::country() const{
  return m_country;
}
void PlayerData::setCountry(const QString s){
  m_country = s;
}

QString PlayerData::title() const{
  return m_title;
}
void PlayerData::setTitle(const QString s){
  m_title = s;
}

int PlayerData::elo(const QDate dt) const{
  if (m_elo.isEmpty()){
    return m_estimatedElo;
  }
  QMap<Q_INT32, QMemArray<int> >::const_iterator iter = m_elo.find((Q_INT32)dt.year());
  if (iter == m_elo.end()){
    --iter;
    if (dt.year() > iter.key()){//date is after latest elo entry
      return iter.data()[iter.data().size() -1];
    }
    else{//date is before earliest elo entry
      return m_peakElo;//anything better? this is what Scid seems to do (Larsen,B fe.)
    }
  }
  else{
//TODO test this with a player that died in 1999 or later, with just one rating entry in his
//last year.
    if(iter.data().isEmpty()){
      return m_peakElo;//anything better ?
    }
    else{
      if (iter.key() >= 2001){// years with 4 elo periods
        if (dt.month()<=3){
          return iter.data()[0];
        }
        else if (dt.month()<=6){
          if (iter.data().size()>1){
            return iter.data()[1];
          }
          else{
            return iter.data()[0];
          }
        }
        else if (dt.month()<=9){
          if (iter.data().size()>2){
            return iter.data()[2];
          }
          else if (iter.data().size()>1){
            return iter.data()[1];
          }
          else{
            return iter.data()[0];
          }
        }
        else{
          if (iter.data().size()>3){
            return iter.data()[3];
          }
          else if (iter.data().size()>2){
            return iter.data()[2];
          }
          else if (iter.data().size()>1){
            return iter.data()[1];
          }
          else{
            return iter.data()[0];
          }
        }
      }
      else if (iter.data().size()==2){//before 2001
        if (dt.month()<=6){
          return iter.data()[0];
        }
        else{
          return iter.data()[1];
        }
      }
      else{//before 2001
        return iter.data()[0];
      }
    }
  }
}

void PlayerData::setOfficialElo(const Q_INT32 year, const QMemArray<int> ar){
   m_elo.insert(year, ar);
}

Q_INT32 PlayerData::peakElo() const{
  return (Q_INT32)m_peakElo;
}
void PlayerData::setPeakElo(const int i){
  m_peakElo = i;
}

Q_INT32 PlayerData::estimatedElo() const{
  return (Q_INT32)m_estimatedElo;
}
void PlayerData::setEstimatedElo(const int i){
  m_estimatedElo = i;
}

QImage PlayerData::photo() const{
  return m_photo;
}
void PlayerData::setPhoto(const QImage img){
  m_photo = img;
}

QString PlayerData::biography() const{
  return m_biography;
}
void PlayerData::setBiography(const QString s){
  m_biography = s;
}
void PlayerData::appendToBiography(const QString s){
  m_biography += s;
}

QValueList<Q_INT32> PlayerData::eloList() {
   QValueList<Q_INT32> list;
   QMap<Q_INT32, QMemArray<int> >::Iterator it;
   Q_INT32 i;
   Q_INT32 sz;
   QMemArray<int> ar;
   for ( it = m_elo.begin(); it != m_elo.end(); ++it ) {
     list.push_back(it.key()); // year
     ar = it.data();
     sz = ar.size();
     list.push_back((Q_INT32)(sz));
     for (i=0; i<sz; i++){
       list.push_back((Q_INT32)(ar[i])); // rating
     }
   }
   return list;
}
